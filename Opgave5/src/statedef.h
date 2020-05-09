/*
    
   !!OBS!!
   
   Denne kode er en tilpasset version
   af den udleverede kode fra filen
   PWM_ex_statemachine_pointer_driven.c

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


//Menu init message
char message[]= "Indtast:\r a to set PWM MIN value (0) \r b to set PWM MAX value (255)\r\0";

//Action function difinitions
void noAction(void);
void init_MIN_MAX(void);
void updateMIN(void);
void updateMAX(void);
void saveChar(void);
void resetBuffer(void);

/*
CurSta		noinput							"a"						"b"						"Enter"
init			init_s,init_MIN_MAX	MIN,noAction	MAX,noAction	listen,noAction
listen		listen,noAction			MIN,noAction	MAX,noAction	listen,noAction
MIN				MIN_val,saveChar		MIN,noAction	MAX,noAction	listen,noAction
MIN_val		MIN_val,saveChar		MIN,noAction	MAX,noAction	listen,updateMIN
MAX				MAX_val,saveChar		MIN,noAction	MAX,noAction	listen,noAction
MAX_val		MAX_val,saveChar		MIN,noAction	MAX,noAction	listen,updateMAX
*/

stateElement stateMatrix[6][4] = {
	{ 	{init_s,init_MIN_MAX},{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{listen,noAction}, 		{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{MIN_val,saveChar},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{MIN_val,saveChar},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,updateMIN} 	},
	{ 	{MAX_val,saveChar},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,noAction} 	},
	{ 	{MAX_val,saveChar},		{MIN,noAction}, 	{MAX,noAction}, 	{listen,updateMAX} 	}
};

//initial event
event	eventOccured = NILEVENT;
void	stateEval(event e);

//initial state
state currentState = init_s;

//state action control based on events
void stateEval(event e)
{
    stateElement stateEvaluation = stateMatrix[currentState][e];     //a table look-up to fetch the relevant next state and the related function pointer
    currentState = stateEvaluation.nextState;
    (*stateEvaluation.actionToDo)();                               //the execution
}
