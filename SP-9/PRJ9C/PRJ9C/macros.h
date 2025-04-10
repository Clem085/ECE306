#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //


#define NOT_OKAY           (0)
#define OKAY               (1)
#define DEBOUNCE_TIME      (10000)
#define DEBOUNCE_RESTART   (0)
#define PRESSED            (0)
#define RELEASED           (1)

#define P4PUD (P4OUT)
#define P2PUD (P2OUT)
#define TB0CCR0_INTERVAL (25000) // 8,000,000 / 2 / 8 / (1 / 50msec)
#define TB0CCR1_INTERVAL (25000)  // 0.5 seconds
#define TB0CCR2_INTERVAL (25000)

#define TWO_SEC (25000000)  //5 sec
#define FIVE_SEC (2000000)  //2 sec

#define ID_1 (0x0040) /* /2 */
#define ID_2 (0x0080) /* /4 */
#define ID_3 (0x00c0) /* /8 */
#define ID__2 (0x0040) /* /2 */
#define ID__4 (0x0080) /* /4 */
#define ID__8 (0x00c0) /* /8 */

#define TBIDEX_0 (0x0000) /* Divide by 1 */
#define TBIDEX_1 (0x0001) /* Divide by 2 */
#define TBIDEX_2 (0x0002) /* Divide by 3 */
#define TBIDEX_3 (0x0003) /* Divide by 4 */
#define TBIDEX_4 (0x0004) /* Divide by 5 */
#define TBIDEX_5 (0x0005) /* Divide by 6 */
#define TBIDEX_6 (0x0006) /* Divide by 7 */
#define TBIDEX_7 (0x0007) /* Divide by 8 */
#define TBIDEX__1 (0x0000) /* Divide by 1 */
#define TBIDEX__2 (0x0001) /* Divide by 2 */
#define TBIDEX__3 (0x0002) /* Divide by 3 */
#define TBIDEX__4 (0x0003) /* Divide by 4 */
#define TBIDEX__5 (0x0004) /* Divide by 5 */
#define TBIDEX__6 (0x0005) /* Divide by 6 */
#define TBIDEX__7 (0x0006) /* Divide by 7 */
#define TBIDEX__8 (0x0007) /* Divide by 8 */
#define FIFTY_MS_COUNT 10
#define TWO_MS_COUNT (2)

#define Time_Sequence_Rate (50) // 50 millisecods
#define S1250 (1250/Time_Sequence_Rate) // 1.25 seconds
#define S1000 (1000/Time_Sequence_Rate) // 1 second
#define S750 (750/Time_Sequence_Rate) // 0.75 seconds
#define S500 (500/Time_Sequence_Rate) // 0.50 seconds
#define S250 (250/Time_Sequence_Rate) // 0.25 seconds
#define S0250 (25000) // 0.025 seconds   50 ms

#define CHANGE_PROBE (P3OUT ^ TEST_PROBE)// Change state of Probe

#define PWM_PERIOD (TB3CCR0)
#define LCD_BACKLITE_DIMING (TB3CCR1)

#define SLOW (35000)
#define SLOWER (15000) ///go to 12k or 15k, change values of detectors
#define FAST (50000)
#define PERCENT_100 (50000)
#define PERCENT_80 (45000)
#define WAITINGTOSTART (20)

#define BR2 (2)
#define BR1 (1)
#define LF (0x0A)
#define Switch1 (1)
#define Switch2 (2)
#define YES (1)
#define WAITING ('X')
#define RECEIVED ('Y')
#define TRANSMIT ('Z')


#define MAX_CMD_LEN 16
