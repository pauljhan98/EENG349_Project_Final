#include <StackArray.h>

StackArray <int> stack;
float output;
boolean newInput = false;

void Arithmetic_Parsing() {
  static byte digit_count = 0;
  int RawInput;
  int power;
  float top_number;
  float power_multiply;
  float base;


  while (Serial.available() >0 && newInput == false) {
    RawInput = Serial.read() - '0';

    switch (RawInput) {
      case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        stack.push(RawInput);
        newInput = false;
        break;

      case 9:
        digit_count = stack.count();
        
        while (!stack.isEmpty ()) {
          power = digit_count - stack.count();
          Serial.print("power: ");
          Serial.print(power);
//          Serial.println("power");
          top_number = stack.pop();
          Serial.print("\ttop_number: ");
          Serial.print(top_number);
//          Serial.println("top_number");
          power_multiply = 1;
          base = 10;
          for (int i = 0; i < power; i++)
            power_multiply *= base;
            /*
          for (;;) {
            if (power & 1)
              power_multiply *= base;
            power >>= 1;
            if (!power)
              break;
            base *= base;
          } //end of for loop
          */
          Serial.print("\tpower_multiply: ");
          Serial.print(power_multiply);
//          Serial.println("power_multiply");          
          output = output + (top_number) * (power_multiply);
          Serial.print("\toutput: ");
          Serial.println(output);
//          Serial.println("output");          
        } //while stack.isEmpty loop end
        newInput = true;
        break;
      default:
        newInput = false;
        break;
    } //switch case end

  }//while Serial.available end

}

//end custom functions


void setup() {
  Serial.begin(9600);
  stack.setPrinter (Serial);
  Serial.println("Integer input test");
}

void loop() {
  Arithmetic_Parsing();
  if (newInput == true) {
    //Serial.println(output);
    //Serial.println(newInput);
    newInput = false;
    Serial.println(output);
    output = 0;
    delay(1000);
  }

} //loop end
