The purpose of these testcases is to make a simple test of the scheduler 
functionallity in order to be able to check if the most basic parts of the 
scheduler is functioning as expected.

Please be aware that the testcases does _not_ test all aspect of the scheduler.

The scheduler test suite consist of the following testcases:

Test Case Name           Description
--------------------------------------------------
put_message1             Send 100 message to itselves

put_message2             Send a message to another task and expect a respons
                         back. This is repeated a number of time.
                         
timed_event1             Start a time and wait for the timer to expire

timed_event2             Start a number of timers and wait for them to expire

cancel_timed_event       Start a number of timers and begin to cancle the
                         timers.

cancel_put_message       Send a message and after a short delay remove the 
                         message again.

broadcast1               Send a broadcast message from the environment. 
                         No data ptr and no free function

broadcast2               Send a broadcast message from the environment. 
                         Data ptr and free function given.

timer_impl1              Test multiple (5) timers and that they are run in the
                         correct order - will stress test the timer functionality

bgint_nomsg_notimer      Test that a bgint is called when no timed events and
                         no messages are pending

bgint_nomsg_timer        Test bgint when no messages are pending, but timed events
                         are in the list

bgint_msg_notimer        Test bgint with pending messages and no timed events

bgint_msg_timer          Test bgints with pending messages and pending timed events

bgint_all                Test that all 8 bgint's work with messages and timed events


There is some functions in the testlib.{c,h} that the testcases is using, this 
functions can be used for other test if needed.

The file test_verdict.{c,h} have defined some functions that the testcases is 
using, these functions is used to put information about the testcases status 
and if the testcase is passed or failed on the screen or another place. These 
functions need to be made for the specified enviroment.

NOTE: The test case bgint_none has been disabled since it was testing a violation
of the bgint API.
