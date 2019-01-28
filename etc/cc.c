int callee(int x, int y, int z) {
  return x + y + z;
}

int caller(void) {
  return callee(1,2,3) + 5;
}

int get_average(int data[10]) {
  int ave = 0;
  for (int i = 0; i < 10; i++) {
    ave += data[i];
  }
  return ave / 10;
}

int average() {
  int data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int ave = get_average(data);
  return ave;
}
