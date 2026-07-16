#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# Algo
#

import random
from collections import deque

from AI.algo.state import State
from AI.algo.constants import FOOD_DURATION
from AI.algo.inventory import Inventory
from AI.commande.look.look import Look
from AI.algo.level_up import LevelUp
from AI.algo.election import ElectionManager
from AI.algo.leader import LeaderManager
from AI.algo.rally import RallyManager
from AI.algo.survival import SurvivalManager
from AI.algo.broadcast import BroadcastParser
from AI.algo.response_handler import ResponseHandler


class Algo:

    def __init__(self):
        self.mode = "none"
        self.map_width = 0
        self.map_height = 0

        self.survival_time = 0
        self.state = State.DEFAULT
        self.turns = 0
        self.level = 1
        self.queue = deque()
        self.pending_moves = deque()
        self.vision = []
        self.inventory = Inventory()
        self.look_cmd = Look()
        self.level_up = LevelUp(self.inventory, self.look_cmd)

        self.id = 0
        self.leader_id = None

        self.target_leader_id = None
        self.is_leader = False
        self.leader_confirmed = False
        self.has_responded = False
        self.pending_hello_response = False
        self.lowest_id_heard = None
        self.election_wait = 0
        self.election_done = False
        
        self.first_look_done = False
        self.is_child = False

        self.leader_stopped = False
        self.turns_since_last_call = 0
        self.turns_since_heartbeat = 0
        self.players_confirmed_turns = 0

        self.ready_for_ritual = False
        self.farming_food = False

        self.last_rally_direction = None
        self.team_name = ""

        self.election = ElectionManager(self)
        self.leader = LeaderManager(self)
        self.rally = RallyManager(self)
        self.survival = SurvivalManager(self)
        self.broadcast = BroadcastParser(self)
        self.response_handler = ResponseHandler(self)

    def next_move(self):
        if self.state in (State.ARRIVED, State.RALLIED):
            return self.rally.next_move()

        self.survival.update_state()

        if self.is_leader and self.ready_for_ritual:
            self.pending_moves.clear()

        if self.pending_moves:
            cmd = self.pending_moves.popleft()
            return cmd

        if self.level == 1:
            return self.rush_level_one()

        if self.level >= 2 and self.state != State.SURVIVAL:
            return self._next_move_level_2_plus()

        if self.state == State.LEADER:
            return self.leader.next_move()

        if self.state == State.SURVIVAL:
            return self.survival.next_move()

        return "Look\n"

    def _next_move_level_2_plus(self):
        if self.pending_hello_response and not self.has_responded:
            self.has_responded = True
            self.pending_hello_response = False
            self.election_wait = 0
            self.election_done = False
            return self.make_broadcast(f"Hello_{self.id}")
            
        self.pending_hello_response = False

        if self.is_leader:
            return self.leader.next_move()

        if self.target_leader_id is not None and self.leader_confirmed:
            return self.survival.next_move()

        if self.has_responded and not self.election_done:
            if self.election.step_election_wait():
                if self.election.try_become_leader():
                    return self.leader.next_move()
            return self.survival.next_move()

        if not self.has_responded:
            self.has_responded = True
            self.election_wait = 0
            self.lowest_id_heard = None
            return self.make_broadcast(f"Hello_{self.id}")

        return self.survival.next_move()

    def rush_level_one(self):
        if self.level_up.phase == "incant":
            plan = self.level_up.level_up_move([], self.level, self.id, self.map_width, self.map_height)
            if plan:
                self.pending_moves.extend(plan[1:])
                return plan[0]

        if not self.vision:
            return "Look\n"

        current_vision = self.vision
        self.vision = []

        plan = self.level_up.level_up_move(
            current_vision, self.level, self.id,
            self.map_width, self.map_height
        )
        
        if plan:
            self.pending_moves.extend(plan[1:])
            return plan[0]

        return "Look\n"

    def make_broadcast(self, text):
        return f"Broadcast [{self.team_name}]{text}\n"

    def farm_target(self, target_object):
        if not self.vision:
            return "Look\n"

        current_vision = self.vision
        self.vision = []

        plan = self.look_cmd.pathfinding_move(
            current_vision, target_object, "take", self.level, self.map_width, self.map_height
        )

        if plan:
            self.pending_moves.extend(plan[1:])
            return plan[0]

        return "Look\n"

    def handle_response(self, response, connection):
        return self.response_handler.handle(response, connection, self.mode)

    def send_command(self, connection, command):
        if command is None:
            return False
        if isinstance(command, list):
            if not command:
                return False
            self.pending_moves.extendleft(reversed(command[1:]))
            command = command[0]
        self.queue.append(command.strip())
        return connection.send(command)

    def start(self, connection, mode, map_size, team_name=""):
        parts = map_size.strip().split()
        self.map_width, self.map_height = int(parts[0]), int(parts[1])
        self.mode = mode
        self.team_name = team_name
        self.id = random.randint(1, 10000)

        if connection.socket is not None:
            connection.send("Inventory\n")

        response = connection.receive()
        self.inventory.parse(response)
        self.survival_time = self.inventory.data.get("food", 0) * FOOD_DURATION + 119

        while connection.socket is not None:
            command = self.next_move()
            if not self.send_command(connection, command):
                return

            response = connection.receive()
            if not self.handle_response(response, connection):
                return

            while self.pending_moves:
                cmd = self.pending_moves.popleft()
                if not self.send_command(connection, cmd):
                    return
                response = connection.receive()
                if not self.handle_response(response, connection):
                    return

            self.turns += 1