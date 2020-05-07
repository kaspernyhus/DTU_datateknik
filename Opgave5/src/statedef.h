/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
UART Task - State Machine
Opgave 5
---------------------------------

*/


//States
typedef enum {
    init_s,
		listen,
    MIN,
		MIN_val,
    MAX,
		MAX_val
} state;

//Events
typedef enum {
    NILEVENT,
    a,
    b,
    enter,
} event;

//a pointer to functions
typedef void (*action)();

//function which test on the event at shift states
void stateEval(event e);

//the next state and actions definition
typedef struct {
    state nextState;
    action actionToDo;
} stateElement;


