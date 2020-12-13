#define FILE_NAME "input.txt"
#define ROW_LEN 7
#define COLUMN_LEN 3


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct SeatAssignment {
  size_t row;
  size_t column;
  size_t id;
}SeatAssignment;

static SeatAssignment *s_seats;
static size_t seat_cnt = 0;

void find_seat(char *bp, size_t seat_idx) {
  size_t upper = 127;
  size_t lower = 0;
  size_t idx = 0;

  SeatAssignment *seat = &s_seats[seat_idx];

  for (; idx < ROW_LEN; idx++) {
    size_t mid = lower + (upper - lower) / 2;
    switch(bp[idx]) {
      case 'F':
        upper = mid;
        seat->row = upper;
        break;
      case 'B':
        lower = mid + 1;
        seat->row = lower;
        break;
      default:
        fprintf(stderr, "Received unexpected char in row parsing: %c", bp[idx]);
        exit(-1);
    }
  }

  upper = 7;
  lower = 0;

  for (; idx < ROW_LEN + COLUMN_LEN; idx++) {
    size_t mid = lower + (upper - lower) / 2;
    switch(bp[idx]) {
      case 'R':
        lower = mid + 1;
        seat->column = lower;
        break;
      case 'L':
        upper = mid;
        seat->column = upper;
        break;
      default:
        fprintf(stderr, "Received unexpected char in column parsing: %c", bp[idx]);
        exit(-1);
    }
  }

  seat->id = seat->row * 8 + seat->column;
}

void locate_missing(size_t s) {
  size_t rows[128];
  size_t columns[8];

  for (size_t i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
    rows[i] = i;
  }

  for (size_t i = 0; i < sizeof(columns) / sizeof(columns[0]); i++) {
    columns[i] = i;
  }

  typedef struct AllSeats {
    SeatAssignment seat;
    bool seen;
  }AllSeats;

  AllSeats all_seats[128 * 8];

  for (size_t i = 0; i < 128; i++) {
    for (size_t j = 0; j < 8; j++) {
      SeatAssignment *s = &all_seats[i * 8 + j].seat;
      s->row = i;
      s->column = j;
    }
  }

  for (size_t i = 0; i < s; i++) {
    size_t r = s_seats[i].row;
    size_t c = s_seats[i].column;

    all_seats[r * 8 + c].seen = true;
  }

  printf("Unseen seats are: \n");

  for (size_t i = 0; i < 128 * 8; i++) {
    if (all_seats[i].seen == false) {
      SeatAssignment s = all_seats[i].seat;
      printf("R: %zu | C: %zu | ID: %zu\n", s.row, s.column, s.row * 8 + s.column);
    }
  }
}

int main() {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  size_t read = 0;

  s_seats = malloc(sizeof(SeatAssignment) * 10);
  seat_cnt = 10;

  fp = fopen(FILE_NAME, "r");


  size_t c = 0;
  while ((read = getline(&line, &len, fp)) != EOF) {
    find_seat(line, c);
    c++;

    if (c >= seat_cnt) {
      s_seats = realloc(s_seats, sizeof(SeatAssignment) * seat_cnt * 10);
      seat_cnt *= 10;
    }

  }

  size_t max_id = 0;
  for (size_t i = 0; i < c; i++) {
    if (s_seats[i].id > max_id) {
      max_id = s_seats[i].id;
    }
  }

  printf("max id: %zu", max_id);
  locate_missing(c);



  free(s_seats);

  return 0;
}