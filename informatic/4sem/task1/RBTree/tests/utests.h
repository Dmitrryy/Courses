/*************************************************************************************************
 *
 *   utests.h
 *
 *   Created by dmitry
 *   09.03.2021
 *
 ***/

#pragma once

#define CALLOC_TESTS
#include "../include/RBTree.h"

int run_all_tests();

void test_calloc();
void test_rbCreate();
void test_rbDestroy();
void test_rbForeach();
void test_rbFind();
void test_rbInsert();
void test_rbErase();
void test_rbEmpty();
void test_rbClear();
void test_rbDump();

void test_random();