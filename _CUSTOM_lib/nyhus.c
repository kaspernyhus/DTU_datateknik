



void print_bin(int c, int bits, int row) {
  char c_buffer;
  int XY = 15 - ((16 - bits) / 2); // prints in the center of the screen

  for (int i=0; i<bits; i++) {
    c_buffer = '0' + !!(c & (1 << i));
    sendCharXY(c_buffer, row, XY-i);
  }
}
