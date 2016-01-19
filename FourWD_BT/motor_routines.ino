
// --------------------------------------------------------------------------- Drive
void all_Go(int s, int d) {

  for ( int jt = 0; jt <= 3; jt++) {
    m_Ctrl(jt, 1, d);

  }
}

void turn(int s, int d) {
  
  switch (d) {

    case 0: //CW
      m_Fwd(0);
      m_Fwd(2);
      m_Rev(1);
      m_Rev(3);
      break;
    case 1: //CCW
      m_Fwd(1);
      m_Fwd(3);
      m_Rev(0);
      m_Rev(2);
      break;
    case 2: // STOP
      all_Go(1, STP);
      break;
    default:
      all_Go(1, STP);
  };
  
}

void m_Ctrl(int m_id, int s, int d ) {
  switch (d) {

    case 0: //CW
      m_Fwd(m_id);
      break;
    case 1: //CCW
      m_Rev(m_id);
      break;
    case 2: // STOP
      m_Stop(m_id);
      break;
    default:
      m_Stop(m_id);
  }
}

void m_Spd(int m_id, int s ) {
  analogWrite( m[m_id][0], s);
}

void m_Fwd(int m_id) {
  digitalWrite(m[m_id][0], HIGH);
  digitalWrite(m[m_id][1], LOW);
  delay(25);
}

void m_Rev(int m_id) {
  digitalWrite(m[m_id][0], LOW);
  digitalWrite(m[m_id][1], HIGH);
  delay(25);
}

void m_Stop(int m_id) {
  digitalWrite(m[m_id][0], LOW);
  digitalWrite(m[m_id][1], LOW);
  delay(25);
}
/*
void steer(int ang ){
  if (ang >0) {m_c(steer,100,CW ) }
  else{ m_c(steer,100,CCW )  }
  delay(abs(ang*10));
  m_c(F_steer,0,STOP );

}
*/


