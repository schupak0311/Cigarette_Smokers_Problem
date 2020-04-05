#include <chrono>
#include <thread>
#include <random>
#include <semaphore.h>
#include <pthread.h>

int sleep_up_to(int ms);
void agent1();
void agent2();
void agent3();
void tobacco_pusher();
void paper_pusher();
void match_pusher();
void smoker_with_tobacco();
void smoker_with_paper();
void smoker_with_matches();

sem_t tobacco;
sem_t paper;
sem_t match;
sem_t agentSem;

bool isTobacco = false;
bool isPaper = false;
bool isMatch = false;

sem_t tobaccoSem;
sem_t paperSem;
sem_t matchSem;
sem_t mutex;

int sleep_up_to(int ms)
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, ms);
    std::this_thread::sleep_for(std::chrono::milliseconds(distr(eng)));
}

void* agent1(void* args)
{
    for (int i = 0; i < 6; i++)
    {
        sleep_up_to(200);
        sem_wait(&agentSem);
        sem_post(&paper);
        sem_post(&tobacco);
    }
}

void* agent2(void* args)
{
    for (int i = 0; i < 6; i++)
    {
        sleep_up_to(200);
        sem_wait(&agentSem);
        sem_post(&tobacco);
        sem_post(&match);
    }
}

void* agent3(void* args)
{
    for (int i = 0; i < 6; i++)
    {
        sleep_up_to(200);
        sem_wait(&agentSem);
        sem_post(&paper);
        sem_post(&match);
    }
}

void* tobacco_pusher(void* args)
{
    for (int i = 0; i < 12; i++)
    {
        sem_wait(&tobacco);
        sem_wait(&mutex);

        if (isPaper)
        {
            isPaper = false;
            sem_post(&matchSem);
        }
        else if (isMatch)
        {
            isMatch = false;
            sem_post(&paperSem);
        }
        else
        {
            isTobacco = true;
        }
        sem_post(&mutex);
    }
}

void* match_pusher(void* args)
{
    for (int i = 0; i < 12; i++)
    {
        sem_wait(&match);
        sem_wait(&mutex);
        
        if (isPaper)
        {
            isPaper = false;
            sem_post(&tobaccoSem);
        }
        else if (isTobacco)
        {
            isTobacco = false;
            sem_post(&paperSem);
        }
        else
        {
            isMatch = true;
        }
        sem_post(&mutex);
    }
}

void* paper_pusher(void* args)
{
    for (int i = 0; i < 12; i++)
    {
        sem_wait(&paper);
        sem_wait(&mutex);
        
        if (isMatch)
        {
            isMatch = false;
            sem_post(&tobaccoSem);
        }
        else if (isTobacco)
        {
            isTobacco = false;
            sem_post(&matchSem);
        }
        else
        {
            isPaper = true;
        }
        sem_post(&mutex);
    }
}

void* smoker_with_tobacco(void* args)
{
    for (int i = 0; i < 3; i++)
    {
        sem_wait(&tobaccoSem);
        sleep_up_to(50);
        sem_post(&agentSem);
        sleep_up_to(50);
    }
}

void* smoker_with_paper(void* args)
{
    for (int i = 0; i < 3; i++)
    {
        sem_wait(&paperSem);
        sleep_up_to(50);
        sem_post(&agentSem);
        sleep_up_to(50);
    }
}

void* smoker_with_matches(void* args)
{
    for (int i = 0; i < 3; i++)
    {
        sem_wait(&matchSem);
        sleep_up_to(50);
        sem_post(&agentSem);
        sleep_up_to(50);
    }
}

int main()
{
    // agent semaphores
    sem_init(&agentSem, 0, 1);
    sem_init(&tobacco, 0, 0);
    sem_init(&paper, 0, 0);
    sem_init(&match, 0, 0);

    // pusher semaphores
    sem_init(&tobaccoSem, 0, 0);
    sem_init(&paperSem, 0, 0);
    sem_init(&matchSem, 0, 0);
    sem_init(&mutex, 0, 1);

    // create the agent threads
    pthread_t agent1_thread;
    pthread_t agent2_thread;
    pthread_t agent3_thread;
    pthread_create(&agent1_thread, NULL, agent1, NULL);
    pthread_create(&agent2_thread, NULL, agent2, NULL);
    pthread_create(&agent3_thread, NULL, agent3, NULL);

    // create the pusher threads
    pthread_t paper_pusher_thread;
    pthread_t match_pusher_thread;
    pthread_t tobacco_pusher_thread;
    pthread_create(&paper_pusher_thread, NULL, paper_pusher, NULL);
    pthread_create(&match_pusher_thread, NULL, match_pusher, NULL);
    pthread_create(&tobacco_pusher_thread, NULL, tobacco_pusher, NULL);

    // create the smoker threads
    pthread_t smoker_with_tobacco_thread1;
    pthread_t smoker_with_tobacco_thread2;
    pthread_create(&smoker_with_tobacco_thread1, NULL, smoker_with_tobacco, NULL);
    pthread_create(&smoker_with_tobacco_thread2, NULL, smoker_with_tobacco, NULL);

    pthread_t smoker_with_paper_thread1;
    pthread_t smoker_with_paper_thread2;
    pthread_create(&smoker_with_paper_thread1, NULL, smoker_with_paper, NULL);
    pthread_create(&smoker_with_paper_thread2, NULL, smoker_with_paper, NULL);

    pthread_t smoker_with_matches_thread1;
    pthread_t smoker_with_matches_thread2;
    pthread_create(&smoker_with_matches_thread1, NULL, smoker_with_matches, NULL);
    pthread_create(&smoker_with_matches_thread2, NULL, smoker_with_matches, NULL);
}
