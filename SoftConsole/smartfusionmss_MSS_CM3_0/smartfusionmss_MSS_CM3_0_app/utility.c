#include <stdlib.h>
#include "utility.h"
#include "drivers/mss_timer/mss_timer.h"

void delay(int ms) {
	MSS_TIM2_init(MSS_TIMER_ONE_SHOT_MODE);
	MSS_TIM2_load_immediate(ms * 100000);
	MSS_TIM2_start();
	while (MSS_TIM2_get_current_value() > 0);
}

int no_of_digits(int num) {
    int digit_count = 0;

    while(num > 0) {
        digit_count++;
        num /= 10;
    }

    return digit_count;
}

char* itoa(int num)
{
    char *str;
    int digit_count = 0;

    if(num < 0)
    {
        num = -1*num;
        digit_count++;
    }

    digit_count += no_of_digits(num);
    str = malloc(sizeof(char)*(digit_count+1));

    str[digit_count] = '\0';

    while(num > 0)
    {
        str[digit_count-1] = num%10 + '0';
        num = num/10;
        digit_count--;
    }

    if(digit_count == 1)
        str[0] = '-';

    return str;
}
