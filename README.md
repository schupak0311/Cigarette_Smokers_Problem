# Cigarette Smokers Problem

The cigarette smokers problem is an operating systems synchronization problem that can be solved using semaphores and lightweight posix threads.

Suppose that a cigarette requires three ingredients: tobacco, a rolling paper and a match. There are three chain
smokers, each of whom has only one of these ingredients with infinite supply. There is an agent who has an
infinite supply of all three ingredients. To make a cigarette, the smoker who has tobacco must have the other
two ingredients, a paper and a match. (You can figure out what happens with the smokers who start with
papers and matches, respectively.) The agent and smokers share a table. The agent randomly generates two
ingredients and notifies the smoker who needs these two ingredients. Once they are taken from the table, the
agent supplies another two. On the other hand, each smoker waits for the agent’s notification. Once notified,
the smoker picks up the ingredients, makes a cigarette, smokes for a while, and goes back to the table waiting
for his next ingredients. The problem is to come up with an algorithm for the smokers using semaphores as
synchronization primitives.
