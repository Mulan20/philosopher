

# Philosophers — 42 Bangkok

> One table. Five philosophers. Five forks. Zero deadlocks.

## About

This repository contains my **philosophers** project completed at **42 Bangkok** (mandatory part only). The goal is to solve the classic **Dining Philosophers problem** using threads and mutexes, while avoiding deadlocks and data races. Each philosopher is a thread, each fork is a mutex, and the program must keep every philosopher alive as long as possible .



## Environment & Compilation

**Requirements:**
- Unix-like system (Linux recommended)
- `cc` or `gcc`
- `make`
- `pthread` library

**Compile:**

```bash
make          # builds the philo executable
make clean    # removes object files
make fclean   # removes object files and the executable
make re       # fclean + all
```

**42 Norm check:**

```bash
norminette
```

## Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

**Arguments:**

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) at the table |
| `time_to_die` | Time in ms a philosopher can go without eating before dying |
| `time_to_eat` | Time in ms a philosopher spends eating |
| `time_to_sleep` | Time in ms a philosopher spends sleeping |
| `number_of_times_each_philosopher_must_eat` | *(Optional)* Simulation stops when every philosopher has eaten this many times |

**Examples:**

```bash
./philo 5 800 200 200
./philo 5 800 200 200 7
./philo 1 800 200 200
```

All times are in **milliseconds**.

## How It Works

1. **Initialization:** Parse arguments, allocate the philosophers, forks (mutexes), and shared state.
2. **Thread creation:** Each philosopher is a `pthread`. Each fork is a `pthread_mutex_t`.
3. **Routine:** Each philosopher loops through:
   - **Think** → **Take forks** → **Eat** → **Release forks** → **Sleep**
4. **Monitoring:** A dedicated monitor thread checks each philosopher's `last_meal_time`. If `current_time - last_meal_time > time_to_die`, the philosopher dies, a death message is printed, and the simulation stops.
5. **Termination:** The simulation ends when a philosopher dies, or when every philosopher has eaten the required number of times (if the optional argument is provided).
6. **Output:** Every state change is printed with a timestamp in milliseconds:
   ```
   timestamp_in_ms philosopher_id state
   ```
   States: `has taken a fork`, `is eating`, `is sleeping`, `is thinking`, `died`.

### Avoiding Deadlocks

The classic deadlock happens when every philosopher grabs their left fork simultaneously and waits forever for the right fork. Solutions used in this project:

- **Fork ordering:** Even-numbered philosophers pick up the right fork first, odd-numbered pick up the left fork first. This breaks the circular wait.
- **Or a waiter/semaphore approach** limiting the number of philosophers who can try to eat at once.

### Avoiding Data Races

- Every shared variable (`last_meal_time`, `meal_count`, `simulation_over`) is protected by a mutex.
- A dedicated `print_mutex` ensures output lines never interleave.
- The death flag is checked atomically before printing any state message.

## What I Learned

- **Threads and mutexes:** Creating threads with `pthread_create`, protecting shared data with `pthread_mutex_lock` / `unlock`.
- **Deadlock prevention:** Understanding the four Coffman conditions and breaking the circular wait.
- **Data race prevention:** Identifying every shared resource and protecting it consistently.
- **Precise timing:** Using `gettimeofday` to measure elapsed time in milliseconds and `usleep` for sleeping.
- **Monitoring:** Separating the simulation logic from the death-detection logic with a monitor thread.
- **The 42 Norm:** No `for` loops, functions ≤ 25 lines, no globals, consistent naming.

## Testing

Common test cases:

1. **Basic run:** `./philo 5 800 200 200` — no philosopher should die.
2. **Death test:** `./philo 4 310 200 100` — one philosopher should die.
3. **Single philosopher:** `./philo 1 800 200 200` — the philosopher dies (only one fork, cannot eat).
4. **Meal limit:** `./philo 5 800 200 200 7` — simulation stops after everyone eats 7 times.
5. **Stress test:** `./philo 200 800 200 200` — no crashes, no deadlocks.
6. **Data race check:** Compile with `-fsanitize=thread` and verify no data races are reported.

## Notes

This project is a personal learning record (mandatory part only). It is not guaranteed to pass the Moulinette. If you are also at 42 Bangkok, try implementing it yourself first — philosophers is the best project for understanding concurrency and synchronization.

## References

- `man 3 pthread_create`
- `man 3 pthread_mutex_init`
- `man 2 gettimeofday`
- `man 3 usleep`
- 42 Bangkok official philosophers subject (Intra)
- 42 Norm documentation

---
