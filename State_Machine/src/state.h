


//States
typedef enum {
    init,
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

//the actions definitions as functions
void noAction(void);
void init_MIN_MAX(void);
void updateMIN(void);
void updateMAX(void);
void echoUART(void);
void resetBuffer(void);
void setLEDbrightness(int PWMvalue);

/*
CS				noinput							"a"						"b"						"Enter"
init			init,init_MIN_MAX		MIN,noAction	MAX,noAction	listen,noAction
listen		listen,noAction			MIN,noAction	MAX,noAction	listen,noAction
MIN				MIN_val,noAction		MIN,noAction	MAX,noAction	listen,noAction
MIN_val		MIN_val,noAction		MIN,noAction	MAX,noAction	listen,updateMIN
MAX				MAX_val,noAction		MIN,noAction	MAX,noAction	listen,noAction
MAX_val		MAX_val,noAction		MIN,noAction	MAX,noAction	listen,updateMAX
*/

stateElement stateMatrix[6][4] = {
	{ 	{init,init_MIN_MAX}, 	{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{listen,noAction}, 		{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{MIN_val,noAction},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{MIN_val,echoUART},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,updateMAX} 	},
	{ 	{MAX_val,noAction},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{MAX_val,echoUART},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,updateMAX} 	}
};

//initial event
event 	eventOccured= NILEVENT;
void  stateEval(event e);
//initial state
state currentState=init;

//state action control based on events
void stateEval(event e)
{
    stateElement stateEvaluation = stateMatrix[currentState][e];     //a table look-up to fetch the relevant next state and the related function pointer
    currentState= stateEvaluation.nextState;
    (*stateEvaluation.actionToDo)();                               //the execution
}