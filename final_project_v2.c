#include "config_1_3_2.h"
#include "pt_cornell_1_3_2.h"
#include "port_expander_brl4.h"
#include "tft_master.h"
#include "tft_gfx.h"
#include <stdlib.h>
#include <math.h>
#include <stdfix.h>

char buffer[60];

volatile unsigned int pwm_on_time0 = 3499;
volatile unsigned int pwm_on_time1 = 999;
volatile unsigned int pwm_on_time2 = 3499;
volatile unsigned int pwm_on_time3 = 3499;
//the unit of xy is cm
volatile double x_axis = 0;
volatile double y_axis = 14;
volatile double a0_value = 0;
volatile double a1_value = 0;
volatile double a2_value = 0;
unsigned int system_time = 0;
unsigned int increase_flag = 0;
volatile double radius;
double font_size = 1;
int state = 0;
//modes
int test_mode = 0;
int line_mode = 0;
char word = '1';


unsigned int sita12;                                                            //select the angle12 table
unsigned int sita0;                                                             //select the angle0 table

volatile int angle1[115] = {3756,3618,3551,3503,3462,3427,3395,3365,3338,3314,3290,3266,3245,3225,3206,3187,3167,3150,3132,3117,3101,3085,3069,3054,3040,3026,3011,2999,2986,2972,2959,2948,2935,2922,2911,2900,2889,2878,2867,2856,2844,2835,2824,2814,2803,2793,2784,2774,2765,2755,2747,2738,2728,2720,2712,2703,2695,2687,2679,2671,2663,2655,2647,2639,2633,2625,2618,2610,2604,2596,2590,2583,2577,2571,2564,2558,2552,2545,2539,2534,2528,2523,2517,2512,2505,2501,2496,2491,2486,2481,2477,2472,2467,2462,2459,2454,2451,2446,2443,2440,2437,2432,2429,2427,2424,2421,2418,2416,2413,2411,2408,2407,2405,2404,2402};
volatile int angle2[115] = {5999,5713,5573,5473,5385,5311,5243,5178,5119,5068,5016,4964,4918,4873,4831,4788,4746,4707,4668,4632,4596,4560,4524,4491,4458,4425,4392,4363,4333,4300,4270,4244,4214,4183,4157,4130,4103,4077,4050,4022,3995,3972,3944,3920,3893,3869,3844,3820,3796,3771,3751,3726,3701,3680,3659,3634,3613,3591,3570,3548,3526,3504,3482,3459,3441,3418,3400,3377,3358,3335,3316,3296,3277,3257,3238,3218,3197,3177,3156,3140,3119,3103,3081,3064,3042,3024,3007,2989,2971,2953,2934,2915,2895,2875,2862,2841,2826,2804,2789,2773,2757,2732,2715,2706,2687,2667,2647,2636,2613,2601,2574,2560,2544,2527,2507};
volatile int angle3[115] = {1256,1404,1477,1530,1576,1615,1651,1686,1717,1745,1773,1801,1826,1851,1874,1897,1920,1942,1964,1984,2004,2024,2044,2062,2081,2099,2118,2135,2152,2171,2188,2203,2220,2238,2253,2269,2285,2300,2316,2332,2348,2362,2379,2393,2409,2424,2438,2453,2468,2483,2495,2511,2526,2539,2552,2568,2581,2595,2608,2622,2636,2650,2664,2679,2691,2705,2717,2732,2745,2760,2773,2786,2799,2812,2826,2839,2853,2867,2882,2893,2908,2919,2935,2947,2963,2975,2988,3001,3014,3028,3042,3056,3071,3086,3097,3113,3124,3141,3153,3166,3179,3199,3213,3221,3236,3253,3270,3279,3299,3310,3333,3346,3360,3376,3394};
volatile double length[115] = {20.2,20.1,20.0,19.9,19.8,19.7,19.6,19.5,19.4,19.3,19.2,19.1,19.0,18.9,18.8,18.7,18.6,18.5,18.4,18.3,18.2,18.1,18.0,17.9,17.8,17.7,17.6,17.5,17.4,17.3,17.2,17.1,17.0,16.9,16.8,16.7,16.6,16.5,16.4,16.3,16.2,16.1,16.0,15.9,15.8,15.7,15.6,15.5,15.4,15.3,15.2,15.1,15.0,14.9,14.8,14.7,14.6,14.5,14.4,14.3,14.2,14.1,14.0,13.9,13.8,13.7,13.6,13.5,13.4,13.3,13.2,13.1,13.0,12.9,12.8,12.7,12.6,12.5,12.4,12.3,12.2,12.1,12.0,11.9,11.8,11.7,11.6,11.5,11.4,11.3,11.2,11.1,11.0,10.9,10.8,10.7,10.6,10.5,10.4,10.3,10.2,10.1,10.0,9.9,9.8,9.7,9.6,9.5,9.4,9.3,9.2,9.1,9.0,8.9,8.8}; 
//tan_value from -89 to 89, step = 0.5
volatile double tan_value[601] = {-1.7321,-1.7182,-1.7045,-1.6909,-1.6775,-1.6643,-1.6512,-1.6383,-1.6255,-1.6128,-1.6003,-1.5880,-1.5757,-1.5637,-1.5517,-1.5399,-1.5282,-1.5166,-1.5051,-1.4938,-1.4826,-1.4715,-1.4605,-1.4496,-1.4388,-1.4281,-1.4176,-1.4071,-1.3968,-1.3865,-1.3764,-1.3663,-1.3564,-1.3465,-1.3367,-1.3270,-1.3175,-1.3079,-1.2985,-1.2892,-1.2799,-1.2708,-1.2617,-1.2527,-1.2437,-1.2349,-1.2261,-1.2174,-1.2088,-1.2002,-1.1918,-1.1833,-1.1750,-1.1667,-1.1585,-1.1504,-1.1423,-1.1343,-1.1263,-1.1184,-1.1106,-1.1028,-1.0951,-1.0875,-1.0799,-1.0724,-1.0649,-1.0575,-1.0501,-1.0428,-1.0355,-1.0283,-1.0212,-1.0141,-1.0070,-1.0000,-0.9930,-0.9861,-0.9793,-0.9725,-0.9657,-0.9590,-0.9523,-0.9457,-0.9391,-0.9325,-0.9260,-0.9195,-0.9131,-0.9067,-0.9004,-0.8941,-0.8878,-0.8816,-0.8754,-0.8693,-0.8632,-0.8571,-0.8511,-0.8451,-0.8391,-0.8332,-0.8273,-0.8214,-0.8156,-0.8098,-0.8040,-0.7983,-0.7926,-0.7869,-0.7813,-0.7757,-0.7701,-0.7646,-0.7590,-0.7536,-0.7481,-0.7427,-0.7373,-0.7319,-0.7265,-0.7212,-0.7159,-0.7107,-0.7054,-0.7002,-0.6950,-0.6899,-0.6847,-0.6796,-0.6745,-0.6694,-0.6644,-0.6594,-0.6544,-0.6494,-0.6445,-0.6395,-0.6346,-0.6297,-0.6249,-0.6200,-0.6152,-0.6104,-0.6056,-0.6009,-0.5961,-0.5914,-0.5867,-0.5820,-0.5774,-0.5727,-0.5681,-0.5635,-0.5589,-0.5543,-0.5498,-0.5452,-0.5407,-0.5362,-0.5317,-0.5272,-0.5228,-0.5184,-0.5139,-0.5095,-0.5051,-0.5008,-0.4964,-0.4921,-0.4877,-0.4834,-0.4791,-0.4748,-0.4706,-0.4663,-0.4621,-0.4578,-0.4536,-0.4494,-0.4452,-0.4411,-0.4369,-0.4327,-0.4286,-0.4245,-0.4204,-0.4163,-0.4122,-0.4081,-0.4040,-0.4000,-0.3959,-0.3919,-0.3879,-0.3839,-0.3799,-0.3759,-0.3719,-0.3679,-0.3640,-0.3600,-0.3561,-0.3522,-0.3482,-0.3443,-0.3404,-0.3365,-0.3327,-0.3288,-0.3249,-0.3211,-0.3172,-0.3134,-0.3096,-0.3057,-0.3019,-0.2981,-0.2943,-0.2905,-0.2867,-0.2830,-0.2792,-0.2754,-0.2717,-0.2679,-0.2642,-0.2605,-0.2568,-0.2530,-0.2493,-0.2456,-0.2419,-0.2382,-0.2345,-0.2309,-0.2272,-0.2235,-0.2199,-0.2162,-0.2126,-0.2089,-0.2053,-0.2016,-0.1980,-0.1944,-0.1908,-0.1871,-0.1835,-0.1799,-0.1763,-0.1727,-0.1691,-0.1655,-0.1620,-0.1584,-0.1548,-0.1512,-0.1477,-0.1441,-0.1405,-0.1370,-0.1334,-0.1299,-0.1263,-0.1228,-0.1192,-0.1157,-0.1122,-0.1086,-0.1051,-0.1016,-0.0981,-0.0945,-0.0910,-0.0875,-0.0840,-0.0805,-0.0769,-0.0734,-0.0699,-0.0664,-0.0629,-0.0594,-0.0559,-0.0524,-0.0489,-0.0454,-0.0419,-0.0384,-0.0349,-0.0314,-0.0279,-0.0244,-0.0209,-0.0175,-0.0140,-0.0105,-0.0070,-0.0035,0.0000,0.0035,0.0070,0.0105,0.0140,0.0175,0.0209,0.0244,0.0279,0.0314,0.0349,0.0384,0.0419,0.0454,0.0489,0.0524,0.0559,0.0594,0.0629,0.0664,0.0699,0.0734,0.0769,0.0805,0.0840,0.0875,0.0910,0.0945,0.0981,0.1016,0.1051,0.1086,0.1122,0.1157,0.1192,0.1228,0.1263,0.1299,0.1334,0.1370,0.1405,0.1441,0.1477,0.1512,0.1548,0.1584,0.1620,0.1655,0.1691,0.1727,0.1763,0.1799,0.1835,0.1871,0.1908,0.1944,0.1980,0.2016,0.2053,0.2089,0.2126,0.2162,0.2199,0.2235,0.2272,0.2309,0.2345,0.2382,0.2419,0.2456,0.2493,0.2530,0.2568,0.2605,0.2642,0.2679,0.2717,0.2754,0.2792,0.2830,0.2867,0.2905,0.2943,0.2981,0.3019,0.3057,0.3096,0.3134,0.3172,0.3211,0.3249,0.3288,0.3327,0.3365,0.3404,0.3443,0.3482,0.3522,0.3561,0.3600,0.3640,0.3679,0.3719,0.3759,0.3799,0.3839,0.3879,0.3919,0.3959,0.4000,0.4040,0.4081,0.4122,0.4163,0.4204,0.4245,0.4286,0.4327,0.4369,0.4411,0.4452,0.4494,0.4536,0.4578,0.4621,0.4663,0.4706,0.4748,0.4791,0.4834,0.4877,0.4921,0.4964,0.5008,0.5051,0.5095,0.5139,0.5184,0.5228,0.5272,0.5317,0.5362,0.5407,0.5452,0.5498,0.5543,0.5589,0.5635,0.5681,0.5727,0.5774,0.5820,0.5867,0.5914,0.5961,0.6009,0.6056,0.6104,0.6152,0.6200,0.6249,0.6297,0.6346,0.6395,0.6445,0.6494,0.6544,0.6594,0.6644,0.6694,0.6745,0.6796,0.6847,0.6899,0.6950,0.7002,0.7054,0.7107,0.7159,0.7212,0.7265,0.7319,0.7373,0.7427,0.7481,0.7536,0.7590,0.7646,0.7701,0.7757,0.7813,0.7869,0.7926,0.7983,0.8040,0.8098,0.8156,0.8214,0.8273,0.8332,0.8391,0.8451,0.8511,0.8571,0.8632,0.8693,0.8754,0.8816,0.8878,0.8941,0.9004,0.9067,0.9131,0.9195,0.9260,0.9325,0.9391,0.9457,0.9523,0.9590,0.9657,0.9725,0.9793,0.9861,0.9930,1.0000,1.0070,1.0141,1.0212,1.0283,1.0355,1.0428,1.0501,1.0575,1.0649,1.0724,1.0799,1.0875,1.0951,1.1028,1.1106,1.1184,1.1263,1.1343,1.1423,1.1504,1.1585,1.1667,1.1750,1.1833,1.1918,1.2002,1.2088,1.2174,1.2261,1.2349,1.2437,1.2527,1.2617,1.2708,1.2799,1.2892,1.2985,1.3079,1.3175,1.3270,1.3367,1.3465,1.3564,1.3663,1.3764,1.3865,1.3968,1.4071,1.4176,1.4281,1.4388,1.4496,1.4605,1.4715,1.4826,1.4938,1.5051,1.5166,1.5282,1.5399,1.5517,1.5637,1.5757,1.5880,1.6003,1.6128,1.6255,1.6383,1.6512,1.6643,1.6775,1.6909,1.7045,1.7182,1.7321}; 
//servo0 pwm on time value: from -89 to 89 step 0.5
volatile int angle0[601] = {1832,1838,1843,1849,1855,1860,1866,1871,1877,1882,1888,1893,1899,1905,1910,1916,1921,1927,1932,1938,1943,1949,1955,1960,1966,1971,1977,1982,1988,1993,1999,2005,2010,2016,2021,2027,2032,2038,2043,2049,2055,2060,2066,2071,2077,2082,2088,2093,2099,2105,2110,2116,2121,2127,2132,2138,2143,2149,2155,2160,2166,2171,2177,2182,2188,2193,2199,2205,2210,2216,2221,2227,2232,2238,2243,2249,2255,2260,2266,2271,2277,2282,2288,2293,2299,2305,2310,2316,2321,2327,2332,2338,2343,2349,2355,2360,2366,2371,2377,2382,2388,2393,2399,2405,2410,2416,2421,2427,2432,2438,2443,2449,2455,2460,2466,2471,2477,2482,2488,2493,2499,2505,2510,2516,2521,2527,2532,2538,2543,2549,2555,2560,2566,2571,2577,2582,2588,2593,2599,2605,2610,2616,2621,2627,2632,2638,2643,2649,2655,2660,2666,2671,2677,2682,2688,2693,2699,2705,2710,2716,2721,2727,2732,2738,2743,2749,2755,2760,2766,2771,2777,2782,2788,2793,2799,2805,2810,2816,2821,2827,2832,2838,2843,2849,2855,2860,2866,2871,2877,2882,2888,2893,2899,2905,2910,2916,2921,2927,2932,2938,2943,2949,2955,2960,2966,2971,2977,2982,2988,2993,2999,3005,3010,3016,3021,3027,3032,3038,3043,3049,3055,3060,3066,3071,3077,3082,3088,3093,3099,3105,3110,3116,3121,3127,3132,3138,3143,3149,3155,3160,3166,3171,3177,3182,3188,3193,3199,3205,3210,3216,3221,3227,3232,3238,3243,3249,3255,3260,3266,3271,3277,3282,3288,3293,3299,3305,3310,3316,3321,3327,3332,3338,3343,3349,3355,3360,3366,3371,3377,3382,3388,3393,3399,3405,3410,3416,3421,3427,3432,3438,3443,3449,3455,3460,3466,3471,3477,3482,3488,3493,3499,3505,3510,3516,3521,3527,3532,3538,3543,3549,3555,3560,3566,3571,3577,3582,3588,3593,3599,3605,3610,3616,3621,3627,3632,3638,3643,3649,3655,3660,3666,3671,3677,3682,3688,3693,3699,3705,3710,3716,3721,3727,3732,3738,3743,3749,3755,3760,3766,3771,3777,3782,3788,3793,3799,3805,3810,3816,3821,3827,3832,3838,3843,3849,3855,3860,3866,3871,3877,3882,3888,3893,3899,3905,3910,3916,3921,3927,3932,3938,3943,3949,3955,3960,3966,3971,3977,3982,3988,3993,3999,4005,4010,4016,4021,4027,4032,4038,4043,4049,4055,4060,4066,4071,4077,4082,4088,4093,4099,4105,4110,4116,4121,4127,4132,4138,4143,4149,4155,4160,4166,4171,4177,4182,4188,4193,4199,4205,4210,4216,4221,4227,4232,4238,4243,4249,4255,4260,4266,4271,4277,4282,4288,4293,4299,4305,4310,4316,4321,4327,4332,4338,4343,4349,4355,4360,4366,4371,4377,4382,4388,4393,4399,4405,4410,4416,4421,4427,4432,4438,4443,4449,4455,4460,4466,4471,4477,4482,4488,4493,4499,4505,4510,4516,4521,4527,4532,4538,4543,4549,4555,4560,4566,4571,4577,4582,4588,4593,4599,4605,4610,4616,4621,4627,4632,4638,4643,4649,4655,4660,4666,4671,4677,4682,4688,4693,4699,4705,4710,4716,4721,4727,4732,4738,4743,4749,4755,4760,4766,4771,4777,4782,4788,4793,4799,4805,4810,4816,4821,4827,4832,4838,4843,4849,4855,4860,4866,4871,4877,4882,4888,4893,4899,4905,4910,4916,4921,4927,4932,4938,4943,4949,4955,4960,4966,4971,4977,4982,4988,4993,4999,5005,5010,5016,5021,5027,5032,5038,5043,5049,5055,5060,5066,5071,5077,5082,5088,5093,5099,5105,5110,5116,5121,5127,5132,5138,5143,5149,5155,5160,5166};


//some functions
int getsita0(double x, double y){
    int k;
    double tan_sita;
    tan_sita = -x/y;
    for(k = 0; k <= 600; k++){
        if(tan_sita >= tan_value[k] && tan_sita <= tan_value[k+1])
            break;
    }
    if(k >= 600)
        k = 600;
    return k;
}

int getsita12(double x, double y){
    int i;
    double radius;
    radius = sqrt(x*x+y*y);
    for(i = 0; i <= 115; i++){
        if(radius <= length[i] && radius >= length[i+1])
            break;
    }
    if(i >= 115)
        i = 115;
    return i;
}

// This movement need to be smooth
void setxy(double x, double y, int down){
    int a,b;
    int i;
    a = getsita12(x,y);
    b = getsita0(x,y);
    SetDCOC2PWM(angle1[a]-500);                                                //servo1
    delay_ms(50);
    SetDCOC3PWM(angle0[b]);                                                 //servo0
    SetDCOC1PWM(angle2[a]);                                                //servo2
    SetDCOC4PWM(angle3[a]+300);                                            //servo3
    delay_ms(50);
    if(down == 1){                                                              //put down the pen
        for(i = 0;i <= 24; i++){
            SetDCOC2PWM(angle1[a]-500+20*i);                                                //servo1
            delay_ms(50);
        }
    }
    delay_ms(1000);
}
//set the cordinate
void writexy(double x, double y){
    int a,b;
    a = getsita12(x,y);
    b = getsita0(x,y);
    SetDCOC3PWM(angle0[b]);                                                 //servo0
    SetDCOC2PWM(angle1[a]);                                                //servo1
    SetDCOC1PWM(angle2[a]);                                                //servo2
    SetDCOC4PWM(angle3[a]+300);                                            //servo3
    delay_ms(100);
}

void drawline(double x1, double y1, double x2, double y2){
    double stepx,stepy;
    int i;
    stepx = (x2-x1)/10.0;
    stepy = (y2-y1)/10.0;
    writexy(x1,y1);
    for(i=0;i<=10;i++){
        x1 = x1 + stepx;
        y1 = y1 + stepy;
        writexy(x1,y1);
    }
    
}



void draw_symbol(char a, double x, double y){
    switch(a){
        case '0':
            setxy(x,y,1);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x,y-font_size*2);
            drawline(x,y-font_size*2,x,y);   
            setxy(x,y,0);
            break;
        case '1':
            setxy(x,y,1);
            drawline(x,y,x,y-font_size*2);
            setxy(x,y-font_size*2,0);
            break;
        case '2':
            setxy(x,y,1);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size);
            drawline(x+font_size,y-font_size,x,y-font_size);
            drawline(x,y-font_size,x,y-font_size*2);   
            drawline(x,y-font_size*2,x+font_size,y-font_size*2);  
            setxy(x+font_size,y-font_size*2,0);
            break;
        case '3':
            setxy(x,y,1);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x,y-font_size*2);
            setxy(x,y-font_size,1);
            drawline(x,y-font_size,x+font_size,y-font_size);    
            setxy(x+font_size,y-font_size,0);
            break;
        case '4':
            setxy(x,y,1);
            drawline(x,y,x,y-font_size);
            drawline(x,y-font_size,x+font_size,y-font_size);
            drawline(x+font_size,y-font_size,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size*2);
            setxy(x+font_size,y-font_size*2,0);
            break;
        case '5':
            setxy(x+font_size,y,1);
            drawline(x+font_size,y,x,y);
            drawline(x,y,x,y-font_size);
            drawline(x,y-font_size,x+font_size,y-font_size);
            drawline(x+font_size,y-font_size,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x,y-font_size*2);
            setxy(x,y-font_size*2,0);
            break;
        case '6':
            setxy(x+font_size,y,1);
            drawline(x+font_size,y,x,y);
            drawline(x,y,x,y-font_size*2);
            drawline(x,y-font_size*2,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x+font_size,y-font_size);
            drawline(x+font_size,y-font_size,x,y-font_size);
            setxy(x,y-font_size,0);
            break;
        case '7':
            setxy(x,y,1);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size*2);
            setxy(x+font_size,y-font_size*2,0);
            break;
        case '8':
            setxy(x,y-font_size,1);
            drawline(x,y-font_size,x,y);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x,y-font_size*2);
            drawline(x,y-font_size*2,x,y-font_size);
            drawline(x,y-font_size,x+font_size,y-font_size);
            setxy(x+font_size,y-font_size,0);
            break;
        case '9':
            setxy(x+font_size,y-font_size,1);
            drawline(x+font_size,y-font_size,x,y-font_size);
            drawline(x,y-font_size,x,y);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x,y-font_size*2);
            setxy(x,y-font_size * 2,0);
            break;
            
            //alphabets
        case 'A':
            setxy(x,y-font_size*2,1);
            drawline(x,y-font_size*2,x+font_size/2,y);
            drawline(x+font_size/2,y,x+font_size,y-font_size*2);
            setxy(x,y-font_size,1);
            drawline(x,y-font_size,x+font_size,y-font_size);
            setxy(x+font_size,y-font_size,0);
            break;
            
        case 'B':
            setxy(x,y,1);
            drawline(x,y,x,y - font_size*2);
            drawline(x,y - font_size*2,x+font_size,y-3 * font_size/2);
            drawline(x+font_size,y-3 * font_size/2,x,y-font_size);
            drawline(x,y-font_size,x + font_size,y-font_size/2);
            drawline(x + font_size,y-font_size/2,x,y);
            setxy(x,y,0);
            break;
            
        case 'C':
            setxy(x+font_size,y,1);
            drawline(x+font_size,y,x,y);
            drawline(x,y,x,y-font_size*2);
            drawline(x,y-font_size*2,x+font_size,y-font_size*2);
            setxy(+font_size,y-font_size*2,0);
            break;
            
        case 'D':
            setxy(x,y,1);
            drawline(x,y,x,y - font_size*2);
            drawline(x,y - font_size*2,x+font_size,y-3 * font_size/2);
            drawline(x+font_size,y-3 * font_size/2,x + font_size,y-font_size/2);
            drawline(x + font_size,y-font_size/2,x ,y);
            setxy(x,y,0);
            break;
            break;
            
        case 'E':
            setxy(x+font_size,y,1);
            drawline(x+font_size,y,x,y);
            drawline(x,y,x,y-font_size*2);
            drawline(x,y-font_size*2,x+font_size,y-font_size*2);
            setxy(x+font_size,y-font_size,1);
            drawline(x+font_size,y-font_size,x,y-font_size);    
            setxy(x,y-font_size,0);
            break;
        
        case 'F':
            setxy(x+font_size,y,1);
            drawline(x+font_size,y,x,y);
            drawline(x,y,x,y-font_size*2);
            setxy(x+font_size,y-font_size,1);
            drawline(x+font_size,y-font_size,x,y-font_size);    
            setxy(x,y-font_size,0);
            break;
            
        case 'G':
            setxy(x+font_size,y,1);
            drawline(x+font_size,y,x,y);
            drawline(x,y,x,y-font_size*2);
            drawline(x,y-font_size*2,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x+font_size,y-font_size);
            drawline(x+font_size,y-font_size,x +font_size/2,y-font_size);
            setxy(x+font_size/2,y-font_size,0);
            break;
            
        case 'H':
            setxy(x,y,1);
            drawline(x,y,x,y-font_size*2);
            setxy(x+font_size,y,1);
            drawline(x + font_size,y,x+font_size,y-font_size*2);
            setxy(x+font_size,y-font_size,1);
            drawline(x+font_size,y-font_size,x,y-font_size);
            setxy(x,y-font_size,0);
            break;
            
        case 'I':
            setxy(x,y,1);
            drawline(x,y,x+font_size,y);
            setxy(x+font_size/2,y,1);
            drawline(x+font_size/2,y,x+font_size/2,y-font_size*2);
            setxy(x+font_size,y-font_size*2,1);
            drawline(x+font_size,y-font_size*2,x,y-font_size*2);    
            setxy(x,y-font_size*2,0);
            break;
            
        case 'J':
            setxy(x+font_size,y,1);
            drawline(x+font_size,y,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x+0.2*font_size,y-font_size*2);
            drawline(x,y-font_size*2,x,y-3*font_size/2);    
            setxy(x,y-font_size,0);
            break;
            
        case 'K':
            setxy(x,y,1);
            drawline(x,y,x,y - font_size*2);
            setxy(x+font_size,y-font_size*2,0);
            drawline(x+font_size,y-font_size*2,x,y-font_size);
            drawline(x,y-font_size,x+font_size,y);
            setxy(x+font_size,y,0);
            break;
            
        case 'L':
            setxy(x,y,1);
            drawline(x,y,x,y-font_size*2);
            drawline(x,y-font_size*2,x+font_size,y-font_size*2);
            setxy(x+font_size,y-font_size*2,0);
            break;
            
        case 'M':
            setxy(x,y-font_size*2,1);
            drawline(x,y-font_size*2,x+0.25*font_size,y);
            drawline(x+0.25*font_size,y,x+0.5*font_size,y-2*font_size);
            drawline(x+0.5*font_size,y-2*font_size,x+0.75*font_size,y);
            drawline(x+0.75*font_size,y,x+font_size,y-2*font_size);
            setxy(x+font_size,y-2*font_size,0);
            break;
            
        case 'N':
            setxy(x,y-font_size*2,1);
            drawline(x,y-font_size*2,x,y);
            drawline(x,y,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x+font_size, y);
            setxy(x+font_size, y,0);
            break;
            
        case 'O':
            setxy(x,y,1);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x,y-font_size*2);
            drawline(x,y-font_size*2,x,y);   
            setxy(x,y,0);
            break;
            
        case 'P':
            setxy(x,y-font_size,1);
            drawline(x,y-font_size,x+font_size,y-font_size);
            drawline(x+font_size,y-font_size,x+font_size,y);
            drawline(x+font_size,y,x,y);
            drawline(x,y,x,y-font_size*2);
            setxy(x,y-font_size*2,0);
            break;
            
        case 'Q':
            setxy(x+font_size,y-font_size,1);
            drawline(x+font_size,y-font_size,x,y-font_size);
            drawline(x,y-font_size,x,y);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size*2);
            setxy(x+font_size,y-font_size*2,0);
            break;
            
        case 'R':
            setxy(x,y-font_size*2,1);
            drawline(x,y-font_size*2,x,y);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size,y-font_size);
            drawline(x+font_size,y-font_size,x,y-font_size);
            drawline(x,y-font_size,x+font_size,y-font_size*2);
            setxy(x+font_size,y-font_size*2,0);
            break;
            
        case 'S':
            setxy(x+font_size,y,1);
            drawline(x+font_size,y,x,y);
            drawline(x,y,x,y-font_size);
            drawline(x,y-font_size,x+font_size,y-font_size);
            drawline(x+font_size,y-font_size,x+font_size,y-font_size*2);
            drawline(x+font_size,y-font_size*2,x,y-font_size*2);
            setxy(x,y-font_size*2,0);
            break;
            
        case 'T':
            setxy(x,y,1);
            drawline(x,y,x+font_size,y);
            setxy(x+font_size/2,y,1);
            drawline(x+font_size/2,y,x+font_size/2,y-2*font_size);
            setxy(x+font_size/2,y-2*font_size,0);
            break;
            
        case 'U':
            setxy(x,y,1);
            drawline(x,y,x,y-2*font_size);
            drawline(x,y-2*font_size,x+font_size,y-2*font_size);
            drawline(x+font_size,y-2*font_size,x+font_size,y);
            setxy(x+font_size,y,0);
            break;
            
        case 'V':
            setxy(x,y,1);
            drawline(x,y,x+1/2*font_size,y-2*font_size);
            drawline(x+1/2*font_size,y-2*font_size,x+font_size,y);
            setxy(x+font_size,y,0);
            break;
            
        case 'W':
            setxy(x,y,1);
            drawline(x,y,x+0.25*font_size,y-2*font_size);
            drawline(x+0.25*font_size,y-2*font_size,x+0.5*font_size,y);
            drawline(x+0.5*font_size,y,x+0.75*font_size,y-2*font_size);
            drawline(x+0.75*font_size,y-2*font_size,x+font_size,y);
            setxy(x+font_size,y,0);
            break;
            
        case 'X':
            setxy(x,y,1);
            drawline(x,y,x+font_size,y-font_size*2);
            setxy(x+font_size,y,1);
            drawline(x+font_size,y,x,y-font_size*2);
            setxy(x,y-font_size*2,0);
            break;
            
        case 'Y':
            setxy(x,y,1);
            drawline(x,y,x+font_size/2,y-font_size);
            drawline(x+font_size/2,y-font_size,x+font_size,y);
            drawline(x+font_size,y,x+font_size/2,y-font_size);
            drawline(x+font_size/2,y-font_size,x+font_size/2,y-font_size*2);
            setxy(x+font_size/2,y-font_size*2,0);
            break;
            
        case 'Z':
            setxy(x,y,1);
            drawline(x,y,x+font_size,y);
            drawline(x+font_size,y,x+font_size/2,y-font_size);
            drawline(x+font_size/2,y-font_size,x,y-font_size*2);
            drawline(x,y-font_size*2,x+font_size,y-font_size*2);
            setxy(x+font_size,y-font_size*2,0);
            break;
    }
}

char num2char(int x){
    char y;
    switch(x){
        case 0: y = '0'; break;
        case 1: y = '1'; break;
        case 2: y = '2'; break;
        case 3: y = '3'; break;
        case 4: y = '4'; break;
        case 5: y = '5'; break;
        case 6: y = '6'; break;
        case 7: y = '7'; break;
        case 8: y = '8'; break;
        case 9: y = '9'; break;
        case 10: y = 'E'; break;
        case 11: y = 'C'; break;
    }
    return y;
}

//void __ISR(_TIMER_2_VECTOR, ipl2) Timer2Handler(void){
//    mT2ClearIntFlag();
//    SetDCOC3PWM(pwm_on_time0);
//    SetDCOC2PWM(pwm_on_time1);
//    SetDCOC1PWM(pwm_on_time2);
//    SetDCOC4PWM(pwm_on_time3);
//}

static struct pt pt_timer, pt_key, pt_serial ;



static PT_THREAD (protothread_timer(struct pt *pt))
{
    PT_BEGIN(pt);
    
      
        while(1) {
            if(test_mode == 1){
                font_size = 1;
                draw_symbol('E',-1,16);
                PT_YIELD_TIME_msec(100);
                draw_symbol('C',0.2,16);
                PT_YIELD_TIME_msec(100);
                draw_symbol('E',1.4,16);
                PT_YIELD_TIME_msec(100);
                draw_symbol('4',-2,13);
                PT_YIELD_TIME_msec(100);
                draw_symbol('7',-0.8,13);
                PT_YIELD_TIME_msec(100);
                draw_symbol('6',0.4,13);
                PT_YIELD_TIME_msec(100);
                draw_symbol('0',1.6,13);
                PT_YIELD_TIME_msec(100);
                test_mode = 0;
            }
            else if(line_mode == 1){
                draw_symbol(word,x_axis,y_axis);
                x_axis = x_axis + font_size*1.2;
            }
            else{
                draw_symbol(word,x_axis,y_axis);
            }
            state = 0;
            PT_YIELD_TIME_msec(100);
            
        } // END WHILE(1)
  PT_END(pt);
} // timer thread


static PT_THREAD (protothread_serial(struct pt *pt))
{
    PT_BEGIN(pt);
      static char cmd[30];
      static double value;
      while(1) {
            // send the prompt via DMA to serial
            cursor_pos(4,1);
            red_text ;
            sprintf(PT_send_buffer,"cmd>");
            // by spawning a print thread
            PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
            clr_right ;
            normal_text ; 
            //spawn a thread to handle terminal input
            // the input thread waits for input
            // -- BUT does NOT block other threads
            // string is returned in "PT_term_buffer"
            // !!!! --- !!!!
            // Choose ONE of the following:
            // PT_GetSerialBuffer or PT_GetMachineBuffer
            // !!!! --- !!!!
            PT_SPAWN(pt, &pt_input, PT_GetSerialBuffer(&pt_input) );
            //
            sscanf(PT_term_buffer, "%s %f", cmd, &value);
            // returns when the thead dies on the termination condition
            // IF using PT_GetSerialBuffer, when <enter> is pushed
            // IF using PT_GetMachineBuffer, could be on timeout
//             if(PT_timeout==0) {
//                 sscanf(PT_term_buffer, "%s %f", cmd, &value);
//             }
//            // no actual string
//             else {
//                 // uncomment to prove time out works
//                 //mPORTAToggleBits(BIT_0);
//                 // disable the command parser below
//                 cmd[0] = 0 ;
//             }

             switch(cmd[0]){
                 case 'x': // set frequency of DAC sawtooth output
                     // enter frequecy in HZ
                    cursor_pos(6,1);
                    x_axis = value;
                    sprintf(PT_send_buffer,"x = %.2f", x_axis);
                    // by spawning a print thread
                    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
                    clr_right ; 
          
                     break;
     
                 case 'y':
                     // send the prompt via DMA to serial console
                    // by spawning a print thread
                    cursor_pos(8,1);
                    y_axis = value;
                    sprintf(PT_send_buffer,"y = %.2f ", y_axis);
                    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
                    clr_right ; 
                    break;
//                    
                case 'w':
                 // send the prompt via DMA to serial console
                // by spawning a print thread
                    cursor_pos(10,1);
                    word = cmd[1];
                    state = 1;
                    sprintf(PT_send_buffer,"word = %c ", word);
                    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
                    clr_right ; 
                break;
                
                case 't':
                 // send the prompt via DMA to serial console
                // by spawning a print thread
                    cursor_pos(10,1);
                    test_mode = 1;
                    state = 1;
                    sprintf(PT_send_buffer,"test_mode!");
                    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
                    clr_right ; 
                break;
                
                case 'f':
                 // send the prompt via DMA to serial console
                // by spawning a print thread
                    cursor_pos(12,1);
                    font_size = value;
                    sprintf(PT_send_buffer,"font size = %.2f", font_size);
                    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
                    clr_right ; 
                break;
                
                case 'l':
                 // send the prompt via DMA to serial console
                // by spawning a print thread
                    cursor_pos(14,1);
                    line_mode = 1;
                    sprintf(PT_send_buffer,"line mode");
                    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
                    clr_right ; 
                break;
                
                case 'e':
                 // send the prompt via DMA to serial console
                // by spawning a print thread
                    cursor_pos(14,1);
                    line_mode = 0;
                    sprintf(PT_send_buffer,"exit line mode");
                    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
                    clr_right ; 
                break;
//                case '2':
//                     // send the prompt via DMA to serial console
//                    // by spawning a print thread
//                    cursor_pos(12,1);
//                    a2_value = value;
//                    sprintf(PT_send_buffer,"a2 = %d ", desired_angle(a2_value));
//                    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
//                    clr_right ; 
//                    break;
                    
//                case 'd':
//                     // send the prompt via DMA to serial console
//                    // by spawning a print thread
//                    cursor_pos(15,1);
//                    d_value = value;
//                    sprintf(PT_send_buffer,"D = %6.1f ", d_value);
//                    PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
//                    clr_right ; 
//                    break;
                    
             }
             
            // never exit while
      } // END WHILE(1)
  PT_END(pt);
} // thread 3



void main(void) {
    ANSELA = 0; ANSELB = 0; 
//use timer3
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_16, 50000);
//    ConfigIntTimer3(T1_INT_ON | T1_INT_PRIOR_2);
    mT2ClearIntFlag(); // and clear the interrupt flag
 // OC3 is PPS group 4, map to RPB9 (pin 18)
    OpenOC3(OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE , pwm_on_time0, pwm_on_time0); //
     PPSOutput(4, RPB9, OC3);
// OC2 is PPS group 2, map to RPB8
     OpenOC2(OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE , angle1[0], angle1[0]); //
     PPSOutput(2, RPB8, OC2);
 // OC1 is PPS group 1, map to RPB7     
     OpenOC1(OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE , angle2[0], angle2[0]); //
     PPSOutput(1, RPB7, OC1);
 // OC4 is PPS group 3, map to RPB2
     OpenOC4(OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE , angle3[0], angle3[0]); //
     PPSOutput(3, RPB2, OC4);
     
     setxy(0, 15, 0);
     
//RB2 is taken, so we didn't use tft
    tft_init_hw();
    tft_begin();
    tft_fillScreen(ILI9340_BLACK);
  //240x320 vertical display
    tft_setRotation(0); // Use tft_setRotation(1) for 320x240
  // === setup system wide interrupts  ========
    INTEnableSystemMultiVectoredInt();
  
    // === config threads ==========
    // turns OFF UART support and debugger pin, unless defines are set
    PT_setup();

    // init the threads
    PT_INIT(&pt_timer);
    PT_INIT(&pt_serial);
    while (1){
        switch(state){
            case 0:
                PT_SCHEDULE(protothread_serial(&pt_serial)); 
                break;
            case 1:
                PT_SCHEDULE(protothread_timer(&pt_timer));
                break;
        }
        
        
    }

      
  } // main






















/*
 * File:        TFT, keypad, DAC, LED, PORT EXPANDER test
 * Direct Digital Synthesis example 
 * DAC channel A has audio
 * DAC channel B has amplitude envelope
 * Author:      Bruce Land
 * For use with Sean Carroll's Big Board
 * Target PIC:  PIC32MX250F128B
 */

//
//#include "config_1_3_2.h"
//// threading library
//#include "pt_cornell_1_3_2.h"
//// yup, the expander
//#include "port_expander_brl4.h"
//// SPI channel 1 connections to TFT
//#include "tft_master.h"
//#include "tft_gfx.h"
//// need for rand function
//#include <stdlib.h>
//// need for sine function
//#include <math.h>
//// The fixed point types
//#include <stdfix.h>
//
//char buffer[60];
//
//// === print a line on TFT =====================================================
//// print a line on the TFT
//// string buffer
//char buffer[60];
////void printLine(int line_number, char* print_buffer, short text_color, short back_color){
////   
////    int v_pos;
////    v_pos = line_number * 10 ;
////    // erase the pixels
////    tft_fillRoundRect(0, v_pos, 239, 8, 1, back_color);// x,y,w,h,radius,color
////    tft_setTextColor(text_color); 
////    tft_setCursor(0, v_pos);
////    tft_setTextSize(1);
////    tft_writeString(print_buffer);
////}
////
////void printLine2(int line_number, char* print_buffer, short text_color, short back_color){
////    // line number 0 to 31 
////    /// !!! assumes tft_setRotation(0);
////    // print_buffer is the string to print
////    int v_pos;
////    v_pos = line_number * 20 ;
////    // erase the pixels
////    tft_fillRoundRect(0, v_pos, 239, 16, 1, back_color);// x,y,w,h,radius,color
////    tft_setTextColor(text_color); 
////    tft_setCursor(0, v_pos);
////    tft_setTextSize(2);
////    tft_writeString(print_buffer);
////}
//
//static struct pt pt_timer, pt_key; ;
//
//// system 1 second interval tick
//int sys_time_seconds ;
//
//// === Timer Thread =================================================
//// update a 1 second tick counter
//static PT_THREAD (protothread_timer(struct pt *pt))
//{
//    PT_BEGIN(pt);
//     // timer readout
//     
//     // set up LED to blink
//     
//      while(1) {
//        // yield time 1 second
//          tft_fillScreen(ILI9340_GREEN);
//        PT_YIELD_TIME_msec(1000) ;
//        sys_time_seconds++ ;
//        tft_fillScreen(ILI9340_BLUE);
//        PT_YIELD_TIME_msec(1000) ;
//      } // END WHILE(1)
//  PT_END(pt);
//} // timer thread
//
//
//
//// === Main  ======================================================
//void main(void) {
// //SYSTEMConfigPerformance(PBCLK);
//  
//  ANSELA = 0; ANSELB = 0; 
//
//  tft_init_hw();
//  tft_begin();
//  tft_fillScreen(ILI9340_BLACK);
//  //240x320 vertical display
//  tft_setRotation(0); // Use tft_setRotation(1) for 320x240
//
//  INTEnableSystemMultiVectoredInt();
//  
//  
//  PT_setup();
//
//  // init the threads
//  PT_INIT(&pt_timer);
//
//  
//  // round-robin scheduler for threads
//  while (1){
//      PT_SCHEDULE(protothread_timer(&pt_timer));
//
//      }
//  } // main
//
//// === end  ======================================================
