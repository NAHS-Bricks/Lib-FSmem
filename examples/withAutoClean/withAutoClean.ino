#include <Arduino.h>
#include <nahs-Bricks-Lib-FSmem.h>

void setup() {
  Serial.begin(115200);

  // Enable cleaning of unused name-spaces
  FSmem.setAutoclean();
  Serial.println();
  FSmem.print();

  // register name-space f
  JsonObject flipflop = FSmem.registerData("f");

  // name-space f is used to keep track of additional name-spaces to register in the different runs
  if (!flipflop.containsKey("s")) flipflop["s"] = false;

  // alter the registered name-spaces (unused ones should be deleted by autoClean)
  if (flipflop["s"]) {
    flipflop["s"] = false;
    FSmem.registerData("t1");
    FSmem.registerData("t2");
  }
  else {
    flipflop["s"] = true;
    FSmem.registerData("t3");
    FSmem.registerData("t4");
  }

  FSmem.print();
  bool written = FSmem.write();
  FSmem.print();
  Serial.print("Written: ");
  Serial.println(written);

  /*
  after the first run the name-spaces f, t3, t4 should be saved
  after the second run the name-spaces f, t1, t2 should be saved
  after the third run again the name-spaces f, t3, t4 should be saved
  after the fourth run again the name-spaces f, t1, t2 should be saved
  and so on...
  */
}

void loop() {
  // put your main code here, to run repeatedly:
}
