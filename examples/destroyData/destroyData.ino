#include <Arduino.h>
#include <nahs-Bricks-Lib-FSmem.h>

// In this example some data is saved to FSmem, and in the next go everything is destroyed (wiped) from FSmem

void setup() {
  Serial.begin(115200);

  Serial.println();
  FSmem.print();

  // register name-space f
  JsonObject flipflop = FSmem.registerData("f");

  // name-space f is used to keep track when to write and when to destroy
  if (!flipflop.containsKey("s")) flipflop["s"] = true;

  // alter saving and destroying data
  if (flipflop["s"]) {
    flipflop["s"] = false;
    FSmem.registerData("t1");
    FSmem.registerData("t2");
  }
  else {
    FSmem.destroy();
  }

  bool written = FSmem.write();
  Serial.print("Written: ");
  Serial.println(written);

  /*
  after the first run the name-spaces f, t1, t2 should be saved
  after the second run nothing should be saved
  after the third run again the name-spaces f, t1, t2 should be saved
  after the fourth run again nothing should be saved
  and so on...
  */
}

void loop() {
  // put your main code here, to run repeatedly:
}
