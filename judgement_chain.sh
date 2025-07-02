#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_header() {
	echo -e "${YELLOW}\n=== $1 ===${NC}"
}

run_test() {
	local line="$1"
	echo -e "${GREEN}Input: [$line]${NC}"

	# 実行結果を一時ファイルに保存
	./minishell -c "$line" 


	echo -e "${BLUE}→ minishell output:${NC}"


	# 差分表示も可能（コメントアウト解除で比較しやすくなる）
	# echo -e "${YELLOW}→ diff output:${NC}"
	# diff -u .sh_out .ms_out || echo -e "${RED}(差分あり)${NC}"

	echo "-----"
}

print_header "🟢 正常に通る構文テスト"
while read -r line; do
	run_test "$line"
done << 'EOF'
echo hello
ls -l | grep mini
cat < Makefile | head -n 1
echo ok > /dev/stdout
echo "hello world" >> /dev/stdout
(echo start && echo mid) || echo fail
( echo a && (echo b || echo c) ) && echo d
(ls | grep a) > /dev/stdout
((ls | grep a)) > /dev/stdout
(ls | grep a) > /dev/stdout && echo "done"
(ls | grep . | sort) > /dev/stdout
((echo foo | grep foo) && echo bar) > /dev/stdout
(((echo a) | (grep a)) && echo ok) > /dev/stdout
(ls *.c | grep out) > /dev/stdout
((ls *.c && echo success) || echo fail) > /dev/stdout
echo hello > "out*"
echo hello > no_match*
echo */*
EOF

print_header "🔴 構文エラー地獄テスト"
while read -r line; do
	run_test "$line"
done << 'EOF'
| 
|| 
&& 
<< 
>> 
< 
> 
(ls 
ls) 
((ls))
(ls -l || | grep foo)
(ls < < infile)
( echo hello && ) 
echo > 
echo < 
echo >> 
cat << 
cat > |
cat < &&
echo < >
(ls -l &&)
( || ls )
(> out.txt)
( (echo hi |) | (ls &&) || (cat <<) )
(ls | grep a) | > out
(ls | grep a) > 
> (ls | grep a)
( ( (ls | grep a) > out )
( (ls > out ) | (grep a) ) >>
((ls | grep a) > out
(((ls))) > > out
((echo a) | (grep b >)) | (echo c)
(((ls | grep a) ||) && echo b)
( (ls | grep a) && (grep b) || ) > out
((ls <) | (grep b > out))
( (ls | grep a) | (grep b | (grep c |)) )
echo hello > *    
echo hello > *a*       
(ls | grep a) > *
EOF

print_header "🎴 テスト完了…キミのミニシェルは耐えられたかな？"
