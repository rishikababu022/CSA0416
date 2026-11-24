#!/bin/bash
# Q1 - Linking, loading and address binding. Run from the directory containing the sources.
CFLAGS="-std=c11 -Wall -Wextra -D_GNU_SOURCE"
rm -f app_static app_dynamic app_dlopen libencrypt.so
echo "=== 1. Build static, dynamic and dlopen versions ==="
gcc $CFLAGS -static -o app_static Q1_app.c Q1_encrypt.c || exit 1
gcc $CFLAGS -fPIC -shared -o libencrypt.so Q1_encrypt.c || exit 1
gcc $CFLAGS -o app_dynamic Q1_app.c -L. -lencrypt -Wl,-rpath,'$ORIGIN' || exit 1
gcc $CFLAGS -o app_dlopen Q1_dlopen_app.c -ldl || exit 1

echo; echo "=== 2. Executable sizes (bytes) ==="
ls -l app_static app_dynamic app_dlopen libencrypt.so | awk '{print $5, $9}'
echo; echo "=== 3. ldd ==="
echo "--- app_static ---";  ldd ./app_static 2>&1
echo "--- app_dynamic ---"; ldd ./app_dynamic
echo "--- app_dlopen (no libencrypt dependency at start-up) ---"; ldd ./app_dlopen

echo; echo "=== 4. Run both, show addresses and /proc/self/maps ==="
echo "--- static ---";  ./app_static maps
echo; echo "--- dynamic ---"; ./app_dynamic maps
echo; echo "--- dlopen ---"; ./app_dlopen maps

echo; echo "=== 5. Start-up time of 1000 runs (ms total) ==="
for v in app_static app_dynamic; do
  s=$(date +%s%N); for i in $(seq 1000); do ./$v >/dev/null; done; e=$(date +%s%N)
  echo "$v: $(( (e - s) / 1000000 )) ms for 1000 runs = $(( (e - s) / 1000000 )) us per run"
done
s=$(date +%s%N); for i in $(seq 1000); do ./app_dlopen noenc >/dev/null; done; e=$(date +%s%N)
echo "app_dlopen (noenc, library never loaded): $(( (e - s) / 1000000 )) us per run"
s=$(date +%s%N); for i in $(seq 1000); do ./app_dlopen >/dev/null; done; e=$(date +%s%N)
echo "app_dlopen (library loaded at run time):  $(( (e - s) / 1000000 )) us per run"

echo; echo "=== 6. Memory use (VmSize / VmRSS in kB) ==="
echo "static:  $(./app_static  | grep -E 'VmSize|VmRSS' | tr -s ' \n' ' ')"
echo "dynamic: $(./app_dynamic | grep -E 'VmSize|VmRSS' | tr -s ' \n' ' ')"
./app_dlopen noenc | head -1
./app_dlopen | grep -E 'before|after'

echo; echo "=== 7. Replace the shared library WITHOUT recompiling the programs ==="
echo "before:"; ./app_dynamic | head -2
gcc $CFLAGS -fPIC -shared -o libencrypt.so Q1_encrypt_v2.c
echo "after replacing libencrypt.so with the security-fixed v2:"; ./app_dynamic | head -2
echo "static version is unchanged (old code is inside the executable):"; ./app_static | head -2
echo "dlopen version also picks up v2:"; ./app_dlopen | grep loaded

echo; echo "=== 8. Two copies running: which pages are shared (from /proc/PID/smaps) ==="
./app_dynamic sleep 3 >/dev/null & P1=$!
./app_dynamic sleep 3 >/dev/null & P2=$!
sleep 1
for p in $P1 $P2; do
  echo "--- PID $p ---"
  awk '/^[0-9a-f]+-[0-9a-f]+ /{name=$6; r=$2} /^Rss:/{rss=$2} /^Pss:/{pss=$2} /^Shared_Clean:/{sc=$2} /^Private_Dirty:/{pd=$2; if(name ~ /libencrypt|libc.so|app_dynamic/) printf "%-45s %s Rss=%skB Pss=%skB Shared_Clean=%skB Private_Dirty=%skB\n", name, r, rss, pss, sc, pd}' /proc/$p/smaps
done
wait
echo "Reading: Pss = Rss/2 on the r-xp mappings of app_dynamic and libencrypt.so means each page is mapped by both processes (one physical copy, cost split)."
echo "libc.so.6 shows Shared_Clean = Rss: its read-only pages are shared by every process on the system."

echo; echo "=== 9. Shared library missing ==="
mv libencrypt.so libencrypt.so.bak
echo "--- app_dynamic (load-time failure, program does not even start) ---"; ./app_dynamic; echo "exit status: $?"
echo "--- app_dlopen (run-time failure handled by the program) ---"; ./app_dlopen; echo "exit status: $?"
echo "--- app_dlopen noenc (library not needed, runs fine) ---"; ./app_dlopen noenc; echo "exit status: $?"
mv libencrypt.so.bak libencrypt.so

echo; echo "=== 10. Position-independent code check ==="
gcc $CFLAGS -c -fPIC Q1_encrypt.c -o /tmp/pic.o && gcc $CFLAGS -c -fno-pic Q1_encrypt.c -o /tmp/nopic.o
echo "relocations in PIC object:"; readelf -r /tmp/pic.o | head -5
echo "relocations in non-PIC object:"; readelf -r /tmp/nopic.o | head -5
file app_dynamic libencrypt.so | cut -c1-120
