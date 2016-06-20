#define LP3_TEST_MODULE CoroutineTest
#include <Lp3/Test.h>

#include <Lp3/Assert.h>
#include <Lp3/LE.h>
#include <Lp3/Engine/Coroutine/CoroutineMacros.h>
#include <Lp3/Engine/Coroutine/CoroutineFinishedException.h>
#include <Lp3/Engine/Coroutine/CoroutineInvalidStateException.h>

#include <boost/preprocessor/slot/slot.hpp>

using namespace Lp3::Engine::Coroutine;


struct Coroutine1
{
    CoroutineState state;
    int i;
    int result;

    Coroutine1()
    :   result(0)
    {
    }

    void operator()() {
        LP3_COROUTINE_BEGIN(state);


        result = 50;
        LP3_YIELD();

        result = 100;


        LP3_COROUTINE_END();
    }
};

LP3_TEST(should_catch_falloff)
{
    Coroutine1 co;
    LP3_ASSERT_EQUAL(0, co.result);
    co();
    LP3_ASSERT_EQUAL(50, co.result);
    co();
    LP3_ASSERT_EQUAL(100, co.result);
    LP3_ASSERT_THROW({ co(); }, CoroutineFinishedException);
}

LP3_TEST(should_catch_invalid_state)
{

    Coroutine1 co;
    co.state.codePointer = -434;
    LP3_ASSERT_THROW( { co(); }, CoroutineInvalidStateException);
}

LP3_TEST(running_handcranked_loop)
{
    struct CO{
        CoroutineState state;
        int i = -1;
        int finished = false;

        void operator() () {
            LP3_COROUTINE_BEGIN(state);

            i = 0;
            LP3_LABEL(start_loop);
                if (i >= 100) goto end_loop;
                LP3_YIELD();
                ++ i; goto start_loop;

            LP3_LABEL(end_loop);
                finished = true;
                LP3_YIELD();

            LP3_COROUTINE_END();
        }
    } co;

    LP3_ASSERT_EQUAL(co.finished, false);
    int lastI = co.i;
    while(!co.finished)
    {
        co();
        LP3_ASSERT_TRUE(lastI < co.i);
    }
    LP3_ASSERT_EQUAL(co.i, 100);
}


LP3_TEST(running_loop_de_loop)
{
    struct CO{
        CoroutineState state;
        int forCodePointer = 0;
        int i = -1;
        int finished = false;
        std::vector<char> letters;

        void operator() () {
            LP3_COROUTINE_BEGIN(state);
                for (i = 0; i < 5; i ++)
                {
                    letters.push_back('a');
                    LP3_YIELD();
                    letters.push_back('b');
                    LP3_YIELD();
                    letters.push_back('c');
                }
                LP3_YIELD();
                finished = true;
            LP3_COROUTINE_END();
        }

        explicit inline operator bool() const {
            return (bool) state;
        }
    } co;

    LP3_ASSERT_EQUAL(co.finished, false);
    LP3_ASSERT_EQUAL((bool) co, true);
    LP3_ASSERT_EQUAL(co.i, -1);
    LP3_ASSERT_EQUAL(co.letters.size(), 0);
    co(); // starts- sets i to 0 and adds 'a'
    LP3_ASSERT_EQUAL(co.i, 0);
    LP3_ASSERT_EQUAL(co.letters.size(), 1);
    co(); // adds 'b'
    LP3_ASSERT_EQUAL(co.i, 0);
    LP3_ASSERT_EQUAL(co.letters.size(), 2);
    co();   // now adds 'c', increments i and adds 'a'
    LP3_ASSERT_EQUAL(co.i, 1);
    LP3_ASSERT_EQUAL(co.letters.size(), 4);

    CO co2; // make a new one so we aren't as confused.
    // Mimic the inner loop
    int letterCount = 0;
    for (int i = 0; i < 5; i ++) {
        co2();
        ++ letterCount;
        LP3_ASSERT_EQUAL(co2.i, i);
        LP3_ASSERT_EQUAL(co2.letters.size(), letterCount);
        co2();
        ++ letterCount;
        LP3_ASSERT_EQUAL(co2.i, i);
        LP3_ASSERT_EQUAL(co2.letters.size(), letterCount);
        // Next loop, we'll get one more letter, so increment letterCount here.
        ++ letterCount;
    }
    LP3_ASSERT_EQUAL(co2.finished, false);
    LP3_ASSERT_EQUAL((bool) co2, true);
    co2();
    // It will add a letter, then exit the loop, but yield.
    LP3_ASSERT_EQUAL(co2.i, 5);
    LP3_ASSERT_EQUAL(co2.letters.size(), 3 * 5);
    LP3_ASSERT_EQUAL(co2.finished, false);
    LP3_ASSERT_EQUAL((bool) co2, true);
    // Now it will end the coroutine.
    co2();
    LP3_ASSERT_EQUAL(co2.i, 5);
    LP3_ASSERT_EQUAL(co2.letters.size(), 3 * 5);


    LP3_ASSERT_EQUAL(co2.finished, true);
    LP3_ASSERT_EQUAL(co2.state.codePointer, -1);
    LP3_ASSERT_EQUAL((bool) co2, false);
}


LP3_TEST(simple_generator)
{
     struct Numbers{
        CoroutineState state;
        int number = 0;

        int operator() () {
            LP3_COROUTINE_BEGIN(state)
                while(true)
                {
                    LP3_YIELD(number);
                    ++ number;
                }
            LP3_COROUTINE_END();
        }
    } numbers;

    LP3_ASSERT_EQUAL(numbers(), 0);
    LP3_ASSERT_EQUAL(numbers(), 1);
    LP3_ASSERT_EQUAL(numbers(), 2);
    LP3_ASSERT_EQUAL(numbers(), 3);
}
