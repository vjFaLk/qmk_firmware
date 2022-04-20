d=keyboards/crkbd/keymaps/vjfalk-miryoku
mkdir $d

for f in users/manna-harbour_miryoku/config.h layouts/community/split_3x6_3/manna-harbour_miryoku/config.h
do
  echo "// copied from $f"
  cat $f
  echo
  echo
done > $d/config.h

for f in users/manna-harbour_miryoku/manna-harbour_miryoku.{h,c}
do
  echo "// copied from $f"
  cat $f
  echo
  echo
done > $d/keymap.c

for f in users/manna-harbour_miryoku/rules.mk
do
  echo "# copied from $f"
  cat $f
done > $d/rules.mk