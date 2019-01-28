int callee(int x, int y, int z) {
  return x + y + z;
}

int caller(void) {
  return callee(1,2,3) + 5;
}
