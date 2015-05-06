//#define DEBUG

#define InstructionWidth    20

#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

#define IN5 6
#define IN6 7
#define IN7 8
#define IN8 9

int table[8][4]=
{
  {LOW, HIGH, HIGH, LOW}, //6
  {LOW, HIGH, LOW, LOW},  //4
  {LOW, HIGH, LOW, HIGH}, //5
  {LOW, LOW, LOW, HIGH}, //1
  {HIGH, LOW, LOW, HIGH}, //9
  {HIGH, LOW, LOW, LOW}, //8
  {HIGH, LOW, HIGH, LOW}, //a
  {LOW, LOW, HIGH, LOW}, //2
};

typedef union
{
  char Char[4];
  long int Int;
}Char_Int;

typedef struct 
{
  char Instruction[InstructionWidth];
  int (*Execute)(int parameter);
}Ins_Exe;

Ins_Exe InsTable[] = 
{
  {"forward", forward},
  {"backward", backward},
  {"turnright", turnright},
  {"turnleft", turnleft},
  {"noop", noop},
};

String CmdString = "";
String ParaString = "";
String *Str;
int left = 0;
int right = 7;
int TableLen = sizeof (InsTable) / sizeof (Ins_Exe);
boolean stringComplete = false;  // whether the string is complete


void setup(void)
{
  Serial.begin(115200);
  CmdString.reserve(200);
  ParaString.reserve(200);
  Str = &CmdString;

#ifdef DEBUG   
  Serial.print("Hello \n");
#endif

  //set IO pinMode and initialise IO
  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, LOW);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN2, LOW);
  pinMode(IN3, OUTPUT);
  digitalWrite(IN3, LOW);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN4, LOW);
  pinMode(IN5, OUTPUT);
  digitalWrite(IN5, LOW);
  pinMode(IN6, OUTPUT);
  digitalWrite(IN6, LOW);
  pinMode(IN7, OUTPUT);
  digitalWrite(IN7, LOW);
  pinMode(IN8, OUTPUT);
  digitalWrite(IN8, LOW);
  
}

void loop(void)
{
  int i = 0;
  if(stringComplete)
  {
#ifdef DEBUG    
    Serial.println(CmdString);
    Serial.println(ParaString);
#endif    
    while (CmdString != InsTable[i].Instruction && i < TableLen - 1)
    {
      i++;
    }
    InsTable[i].Execute(ParaString.toInt());
    Serial.print("$");
    
    CmdString = "";
    ParaString = "";
    stringComplete = false;
    delay(100);
    
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, LOW);
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, LOW);
  }
#ifdef DEBUG
  forward(1000);
  delay(500);
  turnright(256);
  delay(500);
#endif
}

int countDelayms(int times,int total)
{
  int begin_delay = 6500,end_delay = 2400,dur = 100,k;
  k = (begin_delay - end_delay)/dur;
 if(total < 205 || times > total+1)
  return 5000;
 if(times < dur)
  return  (begin_delay - k*times);
 if(times > total - dur)
   return (k*times + begin_delay - k*total);
 return end_delay;
}
//when recieve "!",forwardtoBeEnd uesed for reduction of speed in 3cm
void forwardtoBeEnd()
{
  for (int i = 0; i < 60; i++)
  {
    if (left < 0)
    {
      left = left + 8;
    }
    if (right < 0)
    {
      right = right + 8;
    }
    left = left%8;
    right = right%8;
    digitalWrite(IN1, table[left][3]);
    digitalWrite(IN2, table[left][2]);
    digitalWrite(IN3, table[left][1]);
    digitalWrite(IN4, table[left][0]);
  
    digitalWrite(IN5, table[right][3]);
    digitalWrite(IN6, table[right][2]);
    digitalWrite(IN7, table[right][1]);
    digitalWrite(IN8, table[right][0]);
    left++;
    right--;
    delayMicroseconds(3000 + i * 50);
  }
  //end();
}
//function
int forward(int parameter)
{
  Char_Int feedback;
  for (int i = 0; i < parameter; i++)
  {
    if (Serial.available())
    {
      char inChar = (char)Serial.read();
      if ('!' == inChar)
      {
        forwardtoBeEnd();
        feedback.Int = i+60;
        Serial.print(feedback.Char[0]);
        Serial.print(feedback.Char[1]);
        Serial.print(feedback.Char[2]);
        Serial.print(feedback.Char[3]);
        return 0;
      }
      if ('?' == inChar)
      {
        feedback.Int = i;
        Serial.print(feedback.Char[0]);
        Serial.print(feedback.Char[1]);
        Serial.print(feedback.Char[2]);
        Serial.print(feedback.Char[3]);
        Serial.print('@');
      }
    }
    if (left < 0)
    {
      left = left + 8;
    }
    if (right < 0)
    {
      right = right + 8;
    }
    left = left%8;
    right = right%8;
    digitalWrite(IN1, table[left][3]);
    digitalWrite(IN2, table[left][2]);
    digitalWrite(IN3, table[left][1]);
    digitalWrite(IN4, table[left][0]);
  
    digitalWrite(IN5, table[right][3]);
    digitalWrite(IN6, table[right][2]);
    digitalWrite(IN7, table[right][1]);
    digitalWrite(IN8, table[right][0]);
    left++;
    right--;
    delayMicroseconds(countDelayms(i,parameter));
  }
#ifdef DEBUG
  Serial.println("forward");
#endif
  feedback.Int = parameter;
  Serial.print(feedback.Char[0]);
  Serial.print(feedback.Char[1]);
  Serial.print(feedback.Char[2]);
  Serial.print(feedback.Char[3]);
  return 0;
}

int backward(int parameter)
{
  Char_Int feedback;
  for (int i = 0; i < parameter; i++)
  {
    if (Serial.available())
    {
      char inChar = (char)Serial.read();
      if ('!' == inChar)
      {
        feedback.Int = i;
        Serial.print(feedback.Char[0]);
        Serial.print(feedback.Char[1]);
        Serial.print(feedback.Char[2]);
        Serial.print(feedback.Char[3]);
        return 0;
      }
      if ('?' == inChar)
      {
        feedback.Int = i;
        Serial.print(feedback.Char[0]);
        Serial.print(feedback.Char[1]);
        Serial.print(feedback.Char[2]);
        Serial.print(feedback.Char[3]);
        Serial.print('@');
      }
    }
    if (left < 0)
    {
      left = left + 8;
    }
    if (right < 0)
    {
      right = right + 8;
    }
    left = left%8;
    right = right%8;
    digitalWrite(IN1, table[left][3]);
    digitalWrite(IN2, table[left][2]);
    digitalWrite(IN3, table[left][1]);
    digitalWrite(IN4, table[left][0]);
  
    digitalWrite(IN5, table[right][3]);
    digitalWrite(IN6, table[right][2]);
    digitalWrite(IN7, table[right][1]);
    digitalWrite(IN8, table[right][0]);
    left--;
    right++;
    delayMicroseconds(countDelayms(i,parameter));
  }
#ifdef DEBUG
  Serial.println("backward");
#endif
  feedback.Int = parameter;
  Serial.print(feedback.Char[0]);
  Serial.print(feedback.Char[1]);
  Serial.print(feedback.Char[2]);
  Serial.print(feedback.Char[3]);
  return 0;
}

int turnright(int parameter)
{
  Char_Int feedback;
  for (int i = 0; i < parameter; i++)
  {
    if (Serial.available())
    {
      char inChar = (char)Serial.read();
      if ('!' == inChar)
      {
        feedback.Int = i;
        Serial.print(feedback.Char[0]);
        Serial.print(feedback.Char[1]);
        Serial.print(feedback.Char[2]);
        Serial.print(feedback.Char[3]);
        return 0;
      }
      if ('?' == inChar)
      {
        feedback.Int = i;
        Serial.print(feedback.Char[0]);
        Serial.print(feedback.Char[1]);
        Serial.print(feedback.Char[2]);
        Serial.print(feedback.Char[3]);
        Serial.print('@');
      }
    }
    if (left < 0)
    {
      left = left + 8;
    }
    if (right < 0)
    {
      right = right + 8;
    }
    left = left%8;
    right = right%8;
    digitalWrite(IN1, table[left][3]);
    digitalWrite(IN2, table[left][2]);
    digitalWrite(IN3, table[left][1]);
    digitalWrite(IN4, table[left][0]);
  
    digitalWrite(IN5, table[right][3]);
    digitalWrite(IN6, table[right][2]);
    digitalWrite(IN7, table[right][1]);
    digitalWrite(IN8, table[right][0]);
    left++;
    right++;
    delayMicroseconds(4000);
  }
#ifdef DEBUG
  Serial.println("turnright");
#endif
  feedback.Int = parameter;
  Serial.print(feedback.Char[0]);
  Serial.print(feedback.Char[1]);
  Serial.print(feedback.Char[2]);
  Serial.print(feedback.Char[3]);
  return 0;
}

int turnleft(int parameter)
{
  Char_Int feedback;
  for (int i = 0; i < parameter; i++)
  {
    if (Serial.available())
    {
      char inChar = (char)Serial.read();
      if ('!' == inChar)
      {
        feedback.Int = i;
        Serial.print(feedback.Char[0]);
        Serial.print(feedback.Char[1]);
        Serial.print(feedback.Char[2]);
        Serial.print(feedback.Char[3]);
        return 0;
      }
      if ('?' == inChar)
      {
        feedback.Int = i;
        Serial.print(feedback.Char[0]);
        Serial.print(feedback.Char[1]);
        Serial.print(feedback.Char[2]);
        Serial.print(feedback.Char[3]);
        Serial.print('@');
      }
    }
    if (left < 0)
    {
      left = left + 8;
    }
    if (right < 0)
    {
      right = right + 8;
    }
    left = left%8;
    right = right%8;
    digitalWrite(IN1, table[left][3]);
    digitalWrite(IN2, table[left][2]);
    digitalWrite(IN3, table[left][1]);
    digitalWrite(IN4, table[left][0]);
  
    digitalWrite(IN5, table[right][3]);
    digitalWrite(IN6, table[right][2]);
    digitalWrite(IN7, table[right][1]);
    digitalWrite(IN8, table[right][0]);
    left--;
    right--;
    delayMicroseconds(4000);
  }
#ifdef DEBUG
  Serial.println("turnleft");
#endif
  feedback.Int = parameter;
  Serial.print(feedback.Char[0]);
  Serial.print(feedback.Char[1]);
  Serial.print(feedback.Char[2]);
  Serial.print(feedback.Char[3]);
  return 0;
}

int noop(int parameter)
{
#ifdef DEBUG
  Serial.println("noop");
#endif
  return 0;
}

//interrupt
void serialEvent()
{
  while(Serial.available()){
    char inChar = (char)Serial.read();
    if (inChar == '!')
    {
      break;
    }
    if ('?' == inChar)
    {
      Serial.print('\0');
      Serial.print('\0');
      Serial.print('\0');
      Serial.print('\0');
      Serial.print('@');
      break;
    }
    if (inChar ==' ')
    {
      Str = &ParaString;
    }
    
    if (inChar !=' ' && inChar !='$')
    {
      *Str += inChar;
    }
     
    if(inChar == '$'){
      stringComplete = true;
      Str = &CmdString;
    }
  }
}

