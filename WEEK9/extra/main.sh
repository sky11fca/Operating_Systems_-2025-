 #! /bin/bash
 
 #Start eeny first (it will pause waiting for token)
./eeny &       # Run in background
EENY_PID=$!    # Save PID

# Start meeny with miny's future PID (we don't know it yet)
./meeny &      # Will pause
MEENY_PID=$!

# Start miny with moe's future PID
./miny &       # Will pause
MINY_PID=$!

# Start moe with eeny's PID to complete the ring
./moe $EENY_PID &  # Will pause
MOE_PID=$!

# Now set up the previous links
# Tell miny about moe's PID
kill -SIGSTOP $MINY_PID  # Pause miny temporarily
./miny $MOE_PID &        # Restart miny with correct PID
MINY_PID=$!

# Tell meeny about miny's PID
kill -SIGSTOP $MEENY_PID
./meeny $MINY_PID &
MEENY_PID=$!

# Tell eeny about meeny's PID
kill -SIGSTOP $EENY_PID
./eeny $MEENY_PID &
EENY_PID=$!

# Now start the ring by giving token to eeny
kill -SIGUSR1 $EENY_PID