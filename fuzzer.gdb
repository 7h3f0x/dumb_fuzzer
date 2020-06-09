set height 0
set disassembly-flavor intel
r test.sample
echo \n
x/10i $pc
echo \n
where
echo \n
i r