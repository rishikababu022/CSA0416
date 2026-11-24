#!/bin/sh
# Task 1.2: build the improved directory structure for the college server in ./Task01_demo
# and test the naming rule. Group-based ACLs need root, so this demo uses permission bits
# (and setfacl only if it is available).
set -e
ROOT=Task01_demo
chmod -R u+rwx $ROOT 2>/dev/null || true
rm -rf $ROOT
mkdir -p $ROOT/home $ROOT/projects $ROOT/backup $ROOT/dropbox

# one home directory per student: home/<dept>/<admit-year>/<regno>  (private: 0700)
for dept in CSE ECE; do
  for year in 2024 2025; do
    for n in 001 002; do
      d=$ROOT/home/$dept/$year/${dept}${year}${n}
      mkdir -p "$d/documents" "$d/projects"
      chmod 700 "$d"
    done
  done
done

# shared project folder: group-writable, setgid so new files inherit the project group (2770)
mkdir -p $ROOT/projects/OS-Group-7
chmod 2770 $ROOT/projects/OS-Group-7
echo "report draft" > $ROOT/projects/OS-Group-7/report_v01.txt

# members see the project through a symbolic link in their own home (acyclic graph)
ln -s ../../../../projects/OS-Group-7 $ROOT/home/CSE/2024/CSE2024001/projects/OS-Group-7
ln -s ../../../../projects/OS-Group-7 $ROOT/home/CSE/2024/CSE2024002/projects/OS-Group-7

# drop box for hand-in: everyone may add files, only the owner may delete (sticky bit, 1733)
chmod 1733 $ROOT/dropbox

# read-only nightly backup area
chmod 500 $ROOT/backup

if command -v setfacl >/dev/null 2>&1; then
  setfacl -m "u:$(id -un):rwx" $ROOT/projects/OS-Group-7 2>/dev/null && echo "ACL set with setfacl" || true
fi

echo "== Directory tree =="
find $ROOT | sort | head -n 40
echo; echo "== Permissions of key directories =="
ls -ld $ROOT/home/CSE/2024/CSE2024001 $ROOT/projects/OS-Group-7 $ROOT/dropbox $ROOT/backup
echo; echo "== Link to shared project =="
ls -l $ROOT/home/CSE/2024/CSE2024001/projects/
echo; echo "== Naming rule: <regno>_<course>_<title>_v<NN>.<ext> =="
check() { echo "$1" | grep -Eq '^[A-Z]{3}[0-9]{7}_[A-Z]{3}[0-9]{2}_[A-Za-z0-9-]+_v[0-9]{2}\.[a-z0-9]+$' && echo "OK     $1" || echo "REJECT $1"; }
check CSE2024001_CSA04_assignment4_v01.pdf
check CSE2024001_CSA04_assignment4_v01.docx
check "my file (final).doc"
check CSE2024001_CSA04_notes_v1.txt
chmod -R u+rwx $ROOT
rm -rf $ROOT
