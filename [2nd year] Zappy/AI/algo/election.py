#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# Leader election logic
#

ELECTION_SETTLE_TURNS = 100
TARGET_TEAM_SIZE = 8
MIN_SEEN_BEFORE_FORK = 1


class ElectionManager:

    def __init__(self, agent):
        self.agent = agent
        self.seen_ids = set()
        self.fork_done = False

    def on_hello_broadcast(self, heard_id):
        agent = self.agent

        self.seen_ids.add(heard_id)

        if agent.lowest_id_heard is None or heard_id < agent.lowest_id_heard:
            agent.lowest_id_heard = heard_id
            agent.election_wait = 0
            agent.election_done = False

        self._abdicate_if_needed(heard_id)

        if agent.leader_confirmed:
            return

        if heard_id > agent.id and not agent.has_responded:
            agent.pending_hello_response = True

    def on_leader_broadcast(self, leader_id):
        agent = self.agent

        self.seen_ids.add(leader_id)

        self._abdicate_if_needed(leader_id)

        if agent.is_leader:
            return

        if agent.target_leader_id is None or agent.target_leader_id == str(leader_id):
            agent.target_leader_id = str(leader_id)
            agent.leader_confirmed = True

    def _abdicate_if_needed(self, other_id):
        agent = self.agent

        if not agent.is_leader:
            return
        if other_id >= agent.id:
            return

        agent.is_leader = False
        agent.leader_confirmed = True
        agent.target_leader_id = str(other_id)
        agent.leader_id = None
        agent.election_done = True
        agent.leader_stopped = False
        agent.ready_for_ritual = False
        agent.farming_food = False
        agent.players_confirmed_turns = 0
        agent.pending_moves.clear()

    def try_become_leader(self):

        agent = self.agent

        if agent.lowest_id_heard is not None and agent.lowest_id_heard < agent.id:
            agent.target_leader_id = str(agent.lowest_id_heard)
            agent.election_done = True
            return False

        agent.is_leader = True
        agent.leader_confirmed = True
        agent.target_leader_id = str(agent.id)
        agent.leader_id = agent.id
        agent.election_done = True

        self._check_team_size_and_fork()
        return True

    def step_election_wait(self):
        agent = self.agent
        agent.election_wait += 1
        elapsed = agent.election_wait >= ELECTION_SETTLE_TURNS
        if elapsed:
            agent.election_wait = 0
        return elapsed

    def _check_team_size_and_fork(self):
  
        agent = self.agent

        if self.fork_done:
            return

        total_seen = len(self.seen_ids | {agent.id})
        if total_seen >= TARGET_TEAM_SIZE:
            return

        missing = TARGET_TEAM_SIZE - total_seen
        self.fork_done = True

        for _ in range(missing):
            agent.pending_moves.append(agent.make_broadcast(f"Leader_{agent.id}"))
            agent.pending_moves.append("Fork\n")