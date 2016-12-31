typedef struct {
  int quot, rem;
} _my_div_t;

// Our 8cc doesn't support returning a structure value.
// TODO: update 8cc.
void my_div(unsigned int a, unsigned int b, _my_div_t* o) {
  unsigned int d[24];
  unsigned int r[24];
  unsigned int i;
  r[0] = 1;
  for (i = 0;; i++) {
    d[i] = b;
    unsigned int nb = b + b;
    if (nb > a || nb < b)
      break;
    r[i+1] = r[i] + r[i];
    b = nb;
  }

  unsigned int q = 0;
  for (;; i--) {
    unsigned int v = d[i];
    if (a >= v) {
      q += r[i];
      a -= v;
    }
    if (i == 0)
      break;
  }
  o->quot = q;
  o->rem = a;
}

static int __builtin_mul(int a, int b) {
  int i, e, v;
  int d[24];
  int r[24];
  for (i = 0, e = 1, v = a;; i++) {
    d[i] = v;
    r[i] = e;
    v += v;
    int ne = e + e;
    if (ne < e || ne > b)
      break;
    e = ne;
  }

  int x = 0;
  for (;; i--) {
    if (b >= r[i]) {
      x += d[i];
      b -= r[i];
    }
    if (i == 0)
      break;
  }
  return x;
}

static unsigned int __builtin_div(unsigned int a, unsigned int b) {
  _my_div_t r;
  my_div(a, b, &r);
  return r.quot;
}

static unsigned int __builtin_mod(unsigned int a, unsigned int b) {
  _my_div_t r;
  my_div(a, b, &r);
  return r.rem;
}

int putchar(int c);

static void print_str(const char* p) {
  for (; *p; p++)
    putchar(*p);
}

static char* stringify_int(long v, char* p) {
  *p = '\0';
  do {
    --p;
    *p = v % 10 + '0';
    v /= 10;
  } while (v);
  return p;
}

static void print_int(long v) {
  char buf[32];
  print_str(stringify_int(v, buf + sizeof(buf) - 1));
}

static char* stringify_hex(long v, char* p) {
  int is_negative = 0;
  int c;
  *p = '\0';
  if (v < 0) {
    v = -v;
    is_negative = 1;
  }
  do {
    --p;
    c = v % 16;
    *p = c < 10 ? c + '0' : c - 10 + 'A';
    v /= 16;
  } while (v);
  if (is_negative)
    *--p = '-';
  return p;
}

int main() {
  int i;
  for (i = 1; i <= 100; i++) {
    int done = 0;
    if (i % 3 == 0) {
      putchar('F');
      putchar('i');
      putchar('z');
      putchar('z');
      done = 1;
    }
    if (i % 5 == 0) {
      putchar('B');
      putchar('u');
      putchar('z');
      putchar('z');
      done = 1;
    }

    if (!done)
      print_int(i);
    putchar('\n');
  }
  return 0;
}
