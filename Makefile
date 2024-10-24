TARGET_TEST = tests
TARGET_TEST_FILE = run_tests
CC = gcc

PREF_SRC = ./src/
PREF_OBJ = ./obj/
PREF_TEST = ./tests/

SRC = $(wildcard $(PREF_SRC)*.c)
TEST_SRC = $(wildcard $(PREF_TEST)*c)
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))
TEST_OBJ = $(patsubst $(PREF_TEST)%.c, $(PREF_OBJ)%.o, $(TEST_SRC))

$(TARGET_TEST) : $(OBJ) $(TEST_OBJ)
	$(CC) $(OBJ) $(TEST_OBJ) -o $(TARGET_TEST_FILE) -lcunit -lm

$(PREF_OBJ)%.o : $(PREF_SRC)%.c
	$(CC) -c $< -o $@

$(PREF_OBJ)%.o : $(PREF_TEST)%.c
	$(CC) -c $< -o $@

clean :
	rm $(TARGET_TEST_FILE) $(TARGET_PLAYGROUND) $(PREF_OBJ)*.o
