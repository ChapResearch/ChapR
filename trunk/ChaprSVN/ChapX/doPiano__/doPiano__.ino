void loop(){
  doPiano();
}

void doPiano(){
  int note = readTouchInput;
  for(int x = 0; x < 12; x++){
    if(note == x){
      playNoise('n',x);
      delay(500);
    }
  }
}
