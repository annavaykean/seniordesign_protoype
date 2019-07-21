//goes with other global variables
volatile int Left, Right, Front, Back;

void setup() {
  // put your setup code here, to run once:
Left = 0;
Right = 0;
Front = 0;
Back = 0;
}

void loop() {
  
  // put your main code here, to run repeatedly:
//intialize all counters to 0

Left = 0;
Right = 0;
Front = 0;
Back = 0;


//Code that receives the 4 flags from the mcu, L,R,F,B

//s.read();


/*Code that incrememnts counters for leaning conditions, 
Set to flag if receives 5 consecutive values 
else it resets to 0 */

  if(receive == 'L'){
    Left++;
    Right = 0;
    Front = 0;
    Back = 0;
    }
    else if(receive =='R'){
      Left = 0;
      Right++;
      Front = 0;
      Back = 0;
      }
      else if(receive == 'F'){
        Left = 0;
        Right = 0;
        Front++;
        Back = 0;
        }
        else if(receive == 'B'){
          Left = 0;
          Right = 0;
          Front = 0;
          Back++;
          }

  if(Left == 5){
    //write the flag to firebase
    Left = 0;
    }
    
    if(Right == 5){
      //write the flag to firebase
      Right = 0;
      }
      
      if(Front == 5){
        //write the flag to firebase
        Front = 0;
        }
        
        if(Back == 5){
          //write the flag to firebase
          Back = 0;
          }

}
