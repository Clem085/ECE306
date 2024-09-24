void wait_case(void){
    if(time_change){
        time_change = 0;
        if(delay_start++ >= WAITING2START){
            delay_start = 0;
            state = START;
        }
    }
}

void start_case(void){
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    Forward_On();
    segment_count = 0;
    state = RUN;
}









// Circle
//TRAVEL_DISTANCE = 2
//RIGHT_COUNT_TIME = 7
//LEFT_COUNT_TIME = 8
//WHEEL_COUNT_TIME = 10


switch(event){
case C:
    circle();
    break;
case T:
    triangle();
    break;
case F:
    figure8();
    break
case default:
    break;
}


// Left Circle
    TRAVEL_DISTANCE = 7
    RIGHT_COUNT_TIME = 2s
    LEFT_COUNT_TIME = 0
    WHEEL_COUNT_TIME = 10
    run_case();
}


void run_case(void){
    switch(shape)
    circle:
    if(time_change){
        time_change = 0;
        if(segment_count <= TRAVEL_DISTANCE){
            if(right_motor_count++ >= RIGHT_COUNT_TIME){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= LEFT_COUNT_TIME){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= WHEEL_COUNT_TIME){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Forward_Move();
            }
        }else{
            state = END;
        }
    }
}

void end_case(void){
    Forward_Off();
    state = WAIT;
    event = NONE;
}





circle(void){
// Circle Turn (Right)
    TRAVEL_DISTANCE = 2
    RIGHT_COUNT_TIME = 3
    LEFT_COUNT_TIME = 7
    WHEEL_COUNT_TIME = 10
    run_case();
}

triangle(void){
    for(i = 0; i < 3; i++){
//      Triangle  Turn
        TRAVEL_DISTANCE = 2
        RIGHT_COUNT_TIME = 7
        LEFT_COUNT_TIME = 0
        WHEEL_COUNT_TIME = 10
        run_case();

//      Triangle Straight
        TRAVEL_DISTANCE = 2
        RIGHT_COUNT_TIME = 7
        LEFT_COUNT_TIME = 0
        WHEEL_COUNT_TIME = 10
        run_case();
    }
}

void figure8(void){
//    Right Circle
    TRAVEL_DISTANCE = 2
    RIGHT_COUNT_TIME = 7
    LEFT_COUNT_TIME = 0
    WHEEL_COUNT_TIME = 10
    run_case();
