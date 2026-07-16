/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Unit tests for blacklist functions
*/

#include <criterion/criterion.h>
#include <string.h>
#include <arpa/inet.h>

#include "blacklist.h"

/*
** ─── HELPERS ─────────────────────────────────────────────────────────────────
*/

static struct in_addr make_ip(const char *ip_str)
{
    struct in_addr addr;

    inet_aton(ip_str, &addr);
    return addr;
}

/*
** ─── ban_ip ──────────────────────────────────────────────────────────────────
*/

Test(ban_ip, adds_first_entry)
{
    blacklist_t *head = NULL;
    struct in_addr ip = make_ip("192.168.1.1");

    ban_ip(&head, &ip);
    cr_assert_not_null(head);
    cr_assert_eq(head->ban_ip.s_addr, ip.s_addr);
    destroy_blacklist(head);
}

Test(ban_ip, prepends_new_entry)
{
    blacklist_t *head = NULL;
    struct in_addr ip1 = make_ip("10.0.0.1");
    struct in_addr ip2 = make_ip("10.0.0.2");

    ban_ip(&head, &ip1);
    ban_ip(&head, &ip2);
    cr_assert_eq(head->ban_ip.s_addr, ip2.s_addr);
    cr_assert_eq(head->next->ban_ip.s_addr, ip1.s_addr);
    destroy_blacklist(head);
}

Test(ban_ip, three_entries_form_correct_list)
{
    blacklist_t *head = NULL;
    struct in_addr a = make_ip("1.1.1.1");
    struct in_addr b = make_ip("2.2.2.2");
    struct in_addr c = make_ip("3.3.3.3");

    ban_ip(&head, &a);
    ban_ip(&head, &b);
    ban_ip(&head, &c);
    cr_assert_not_null(head);
    cr_assert_not_null(head->next);
    cr_assert_not_null(head->next->next);
    cr_assert_null(head->next->next->next);
    destroy_blacklist(head);
}

/*
** ─── destroy_blacklist ───────────────────────────────────────────────────────
*/

Test(destroy_blacklist, handles_null)
{
    /* must not crash */
    destroy_blacklist(NULL);
}

Test(destroy_blacklist, frees_single_entry)
{
    blacklist_t *head = NULL;
    struct in_addr ip = make_ip("1.2.3.4");

    ban_ip(&head, &ip);
    destroy_blacklist(head);
    /* valgrind verifies no leak */
}

Test(destroy_blacklist, frees_chain)
{
    blacklist_t *head = NULL;
    struct in_addr a = make_ip("1.0.0.1");
    struct in_addr b = make_ip("1.0.0.2");
    struct in_addr c = make_ip("1.0.0.3");

    ban_ip(&head, &a);
    ban_ip(&head, &b);
    ban_ip(&head, &c);
    destroy_blacklist(head);
}

/*
** ─── is_banned ───────────────────────────────────────────────────────────────
*/

Test(is_banned, returns_false_on_null_list)
{
    struct in_addr ip = make_ip("9.9.9.9");

    cr_assert_eq(is_banned(NULL, &ip), false);
}

Test(is_banned, finds_banned_ip)
{
    blacklist_t *head = NULL;
    struct in_addr ip = make_ip("5.5.5.5");

    ban_ip(&head, &ip);
    cr_assert_eq(is_banned(head, &ip), true);
    destroy_blacklist(head);
}

Test(is_banned, returns_false_for_different_ip)
{
    blacklist_t *head = NULL;
    struct in_addr banned = make_ip("6.6.6.6");
    struct in_addr other = make_ip("7.7.7.7");

    ban_ip(&head, &banned);
    cr_assert_eq(is_banned(head, &other), false);
    destroy_blacklist(head);
}

Test(is_banned, finds_ip_in_middle_of_list)
{
    blacklist_t *head = NULL;
    struct in_addr a = make_ip("1.0.0.1");
    struct in_addr b = make_ip("1.0.0.2");
    struct in_addr c = make_ip("1.0.0.3");

    ban_ip(&head, &a);
    ban_ip(&head, &b);
    ban_ip(&head, &c);
    cr_assert_eq(is_banned(head, &b), true);
    destroy_blacklist(head);
}

Test(is_banned, returns_false_for_unknown_ip_in_list)
{
    blacklist_t *head = NULL;
    struct in_addr a = make_ip("2.2.2.2");
    struct in_addr unknown = make_ip("8.8.8.8");

    ban_ip(&head, &a);
    cr_assert_eq(is_banned(head, &unknown), false);
    destroy_blacklist(head);
}

Test(is_banned, same_ip_banned_twice_still_found)
{
    blacklist_t *head = NULL;
    struct in_addr ip = make_ip("3.3.3.3");

    ban_ip(&head, &ip);
    ban_ip(&head, &ip);
    cr_assert_eq(is_banned(head, &ip), true);
    destroy_blacklist(head);
}
