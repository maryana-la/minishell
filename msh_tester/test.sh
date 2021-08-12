# MINISHELL-TESTER-2020.08

RESET="\033[0m"
BLACK="\033[30m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"

BOLDBLACK="\033[1m\033[30m"
BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
BOLDYELLOW="\033[1m\033[33m"
BOLDBLUE="\033[1m\033[34m"
BOLDMAGENTA="\033[1m\033[35m"
BOLDCYAN="\033[1m\033[36m"
BOLDWHITE="\033[1m\033[37m"

# Compile and set executable rights
make -C ../ > /dev/null
cp ../minishell .
chmod 755 minishell


GOOD=0
TOTAL=0

pipe=/tmp/testpipe
trap "rm -f $pipe" EXIT
if [[ ! -p $pipe ]]; then
    mkfifo $pipe
fi

function exec_test()
{
  ./minishell < $pipe >msh_log 2>&-  &
  IFS=';' read -ra CMND <<< "$@"
  for command in "${CMND[@]}"; do
    # echo $command
    echo $command > $pipe
  done
  echo 'exit' > $pipe
  sleep 0.02
  wait $!
  ES_1=$?
  TEST1=$(cat msh_log)

  TEST2=$(echo $@ | bash 2>&-)
  ES_2=$?
#  if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
  if [ "$TEST1" == "$TEST2" ]; then

    printf "$BOLDGREEN%s$RESET" "✓ "
    ((GOOD++))
  else
    printf "\n$BOLDRED%s$RESET" "✗ "
    printf "$CYAN \"$@\" $RESET"
  fi
  if [ "$TEST1" != "$TEST2" ]; then
    echo
    printf $BOLDRED"Your output : \n%.20s\n$BOLDRED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
    printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGREEN$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
    echo
  fi
#  if [ "$ES_1" != "$ES_2" ]; then
#    echo
#    printf $BOLDRED"Your exit status : $BOLDRED$ES_1$RESET\n"
#    printf $BOLDGREEN"Expected exit status : $BOLDGREEN$ES_2$RESET\n"
#    echo
#  fi
  ((TOTAL++))
  sleep 0.08
}

if [ "$1" == "" ] || [ "$1" == "help" ]; then
  printf "$BOLDMAGENTA\n\tAvailable arg: \n$YELLOW\tall$RESET echo cd pipe env export redirect multi syntax exit\n\
      $GREEN\tbonus$RESET quote wildcard oper\n"
fi
if [ "$1" == "all" ]; then
  printf "$BOLDMAGENTA    _____ _ _ _____ _____ _ _ ______ _ _ \n"
  printf "| \/ |_ _| \ | |_ _|/ ____| | | | ____| | | | \n"
  printf "| \ / | | | | \| | | | | (___ | || | |  | | | | \n"
  printf "| |\/| | | | | . \` | | | \___ \|   | | | | | | \n"
  printf "| | | |_| |_| |\ |_| |_ ____) | | | | |____| |____| |____ \n"
  printf "|_| |_|_____|_| \_|_____|_____/|_| |_|______|______|______|\n$RESET"
fi

# My tests
if [ "$1" == "my" ] || [ "$1" == "all" ]; then
    printf $BOLDMAGENTA"\n\tMY TESTS\n"$RESET
    exec_test 'echo bonjour | |'
    exec_test '|'
    exec_test 'echo bonjour > test\ 1'
    exec_test 'cd $HOME/Documents'
#    exec_test 'echo "\s" & echo "\s"'
    exec_test 'echo >'
    exec_test 'echo -n -n -nnnn -nnnnm'
    exec_test 'cat /dev/random | head -n 1 | cat -e'
    exec_test 'unset var1'
    exec_test 'export "" et unset ""'
    exec_test 'echo test > file test1'
    exec_test '$'
    exec_test 'not_cmd bonjour > salut'
    exec_test 'env    '
    exec_test 'cat Makefile | grep pr | head -n 5 | cd test'
    exec_test 'cat Makefile | grep pr | head -n 5 | cat test'
    exec_test 'cat Makefile | grep pr | head -n 5 | hello'
    exec_test 'echo bonjour >>> test'
    exec_test 'echo bonjour > > out'
    exec_test 'echo 2 >> out1 > out2'
    exec_test 'echo 2 > out1 >> out2'
    exec_test 'cat < test'
    exec_test 'echo bonjour > $test'
#    exec_test 'makefile'
#    exec_test 'cat | cat | cat | ls'
    exec_test '$bla'
    exec_test 'export var ="cat Makefile | grep >"'
    exec_test 'export "test=ici"=coucou'
    exec_test 'c$var Makefile # with var=at'
    exec_test '$LESS$VAR'
    exec_test '/bin/echo bonjour'
    exec_test 'dfdfdf'
    exec_test 'sleep 1 | exit'
    exec_test 'echo bonjour > $test'
    exec_test '"exit retour a la ligne"'
    exec_test 'minishell'
    exec_test 'cat diufosgid'
    exec_test 'exit'
    exec_test 'exit -10'
    exec_test 'exit +10'
    exec_test 'echo coucou |'
    exec_test 'echo "$HOME"'
    exec_test 'echo '$HOME''
    exec_test 'echo $HOME'
    exec_test '> log echo coucou'
    exec_test 'echo hudifg d | | hugdfihd'
    exec_test 'echo'
    exec_test 'echo simple'
    exec_test 'echo -n simple'
    exec_test 'echo '''
    exec_test 'echo ""'
    exec_test 'echo "\"'
    exec_test 'echo "\n \n \n"'
    exec_test 'echo "\n \n \\n"'
    exec_test 'cd'
    exec_test 'cd .'
#    exec_test 'cd ~'
    exec_test 'cd not_valid_file'
    exec_test 'cd a b c d'
    exec_test 'pwd a'
    exec_test 'pwd a b c d'
    exec_test 'export LOL=lala ROR=rara'
    exec_test 'unset LOL ROR'
    exec_test 'unset PWD OLDPWD'
    exec_test 'export "HI= hi"'
    exec_test 'export "HI =hi"'
    exec_test '/bin/ls'
    exec_test 'echo $?'
    exec_test 'echo |'
    exec_test '| echo'
    exec_test 'ls | sort'
    exec_test 'cat < >'
    exec_test 'cat < <'
    exec_test 'cat > >'
#    exec_test 'cat | cat | echo heh'
    exec_test '> a ls > b < Makefile'
    exec_test 'echo > a Hello World!'
    exec_test '> a echo Hello World!'
    exec_test 'cat < Makefile | grep gcc > output'
    exec_test 'exit 0 | exit 1'
    exec_test 'exit 1 | exit 0'
    exec_test 'yes | head'

#    ls -lAR / | cat | grep '/'
fi


# ECHO TESTS
if [ "$1" == "echo" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tECHO TESTS\n"$RESET
  exec_test 'echo test tout'
  exec_test 'echo test tout'
  exec_test 'echo -n test tout'
  exec_test 'echo -n -n -n test tout'
fi

# CD TESTS
if [ "$1" == "cd" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tCD TESTS\n"$RESET
#  exec_test 'cd .. ;pwd '
#  exec_test 'cd /Users ; pwd'
#  exec_test 'cd ; pwd'
#  exec_test 'cd . ; pwd'
#  exec_test 'mkdir test_dir ; cd test_dir ; rm -rf ../test_dir ; cd . ; pwd ; cd .. ; pwd'
fi



# PIPE TESTS
if [ "$1" == "pipe" ] || [ "$1" == "all" ]; then
 printf $BOLDMAGENTA"\n\tPIPE TESTS\n"$RESET
 exec_test '/bin/ls'
	exec_test 'cat tests/lorem.txt | grep arcu | cat -e'
	exec_test 'echo test | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e'
	 exec_test 'cat /dev/random | head -c 100 | wc -c'
	exec_test 'ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls'
 head -c 2000000 /dev/zero > big_file
 exec_test ' cat big_file | ls'
 exec_test ' cat big_file | echo lol'
fi


# ENV EXPANSIONS
if [ "$1" == "env" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tENV EXPANSIONS TESTS\n"$RESET
	exec_test 'echo test test'
	exec_test 'echo test'
	exec_test 'echo $TEST'
	exec_test 'echo "$TEST"'
	exec_test "echo '$TEST'"
	exec_test 'echo "$TEST$TEST$TEST"'
	exec_test 'echo "$TEST$TEST=lol$TEST"'
	exec_test 'echo " $TEST lol $TEST"'
	exec_test 'echo $TEST$TEST$TEST'
	exec_test 'echo $TEST$TEST=lol$TEST""lol'
	exec_test 'echo $TEST lol $TEST'
	exec_test 'echo test "" test "" test'
	exec_test 'echo "$=TEST"'
	exec_test 'echo "$"'
	exec_test 'echo "$?TEST"'
	exec_test 'echo $TEST $TEST'
	exec_test 'echo "$1TEST"'
	exec_test 'echo "$T1TEST"'
fi

# EXPORT
if [ "$1" == "export" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tEXPORT TESTS\n"$RESET
  ENV_SHOW="env | sort | grep -v SHLVL | grep -v _="
  EXPORT_SHOW="export | sort | grep -v SHLVL | grep -v _= | grep -v OLDPWD"
  exec_test 'export ='
#  exec_test 'export 1TEST= ;' $ENV_SHOW
#  exec_test 'export TEST ;' $EXPORT_SHOW
  exec_test 'export ""=""' $ENV_SHOW
  exec_test 'export TES=T=""' $ENV_SHOW
  exec_test 'export TE+S=T=""' $ENV_SHOW
  exec_test 'export TEST=LOL ' $ENV_SHOW
  exec_test 'export TEST=LOL'
  exec_test 'echo $TEST$TEST$TEST=lol$TEST'
  exec_test $ENV_SHOW
  exec_test $EXPORT_SHOW
  exec_test 'export TEST="ls -l -a"'
  exec_test '$TEST'
fi


# REDIRECTIONS
if [ "$1" == "redirect" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tREDIRECTION TESTS\n"$RESET
#  exec_test 'echo test > ls ; cat ls'
#  exec_test 'echo test > ls >> ls >> ls ; echo test >> ls ; cat ls'
#  exec_test '> lol echo test lol ; cat lol'
#  exec_test '>lol echo > test>lol>test>>lol>test mdr >lol test >test ; cat test'
  exec_test 'cat < ls'
##  exec_test 'rm -f ls; cat > ls < ls; rm -f ls'
  exec_test 'ls > ls'
  exec_test 'cat <ls'
  exec_test 'cat <test.sh <ls'
  exec_test 'cat << 123'
  exec_test 'ls | cat -e | nl | rev |cat -e | rev >> ls'
#  exec_test 'cat << stop\n1\EOF\nstopa\nstop'
#  exec_test 'cat <test.sh <<stop \n1\nstop'
#  exec_test 'cat <<stop<ls  \n1\nstop'
#  exec_test 'cat <test.sh << stop1 <<stop2 \na\n \nb \nc \nstop1\n run2 \nstop2'
  exec_test 'rm -f ls >ls'
fi


# MULTI TESTS
if [ "$1" == "multi" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tMULTI TESTS\n"$RESET
  exec_test 'echo testing multi >lol'
# exec_test ' echo <lol <lola ; echo "test 1  | and 2" >>lol ; cat <lol ; cat ../Makefile <lol | grep minishell'
fi

# SYNTAX 
if [ "$1" == "syntax" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tSYNTAX\n"$RESET
  exec_test ''
  exec_test '/bin/lsa'
  exec_test './Makefile'
  exec_test '| test'
  exec_test 'echo > <'
  exec_test 'echo | |'
  exec_test 'echo "||"'
  exec_test '<'
#  exec_test 'rm -f ls; cat < ls > ls'
  exec_test "grep -z"
  exec_test "ls'| 'wc -l"
fi

# EXIT
if [ "$1" == "exit" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tEXIT\n"$RESET
  exec_test "exit 42"
  exec_test "exit 42 53 68"
  exec_test "exit 259"
  exec_test "exit 9223372036854775807"
  exec_test "exit -9223372036854775808"
  exec_test "exit 9223372036854775808"
  exec_test "exit -9223372036854775810"
  exec_test "exit -4"
  exec_test "exit wrong"
  exec_test "exit wrong_command"
  exec_test "gdagadgag"
  exec_test "ls -Z"
  exec_test "cd gdhahahad"
  exec_test "ls -la | wtf"
fi



## BONUS QUOTES
#if [ "$1" == "bonus" ] || [ "$1" == "quote" ]; then
#  Compile and set executable rights
#  make bonus -C ../ > /dev/null
#  cp ../minishell .
#  chmod 755 minishell
#
#  printf $BOLDMAGENTA"\n\tBONUS QUOTE\n"$RESET
#  exec_test "echo '"$USER"'"
#  exec_test "echo "'$USER'""
#fi
#
#
## BONUS WILDCARD
#if [ "$1" == "bonus" ] || [ "$1" == "wildcard" ]; then
#  Compile and set executable rights
#  make bonus -C ../ > /dev/null
#  cp ../minishell .
#  chmod 755 minishell
#
#  printf $BOLDMAGENTA"\n\tBONUS WILDCARD\n"$RESET
#  exec_test "echo * | wc"
##  exec_test "cd .. ; echo * | wc"
#  exec_test "echo .* | wc"
#  exec_test "echo M*e"
#  exec_test "echo *a*e"
#  exec_test "echo *.mp3"
##  exec_test "mkdir empty; cd empty; pwd; echo *; cd ..; rm -rf empty"
#fi
#
## BONUS OPERATOR && || ()
#if [ "$1" == "bonus" ] || [ "$1" == "oper" ]; then
#  Compile and set executable rights
#  make bonus -C ../ > /dev/null
#  cp ../minishell .
#  chmod 755 minishell
#
#  printf $BOLDMAGENTA"\n\tBONUS OPERATOR \$\$ || () \n"$RESET
#  exec_test "true && ls"
#  exec_test "false&&ls"
#  exec_test "true||ls"
#  exec_test "false || ls"
#  exec_test "true || echo 1 && echo 2"
#  exec_test "false || echo 1 && echo 2"
#  exec_test "true || (echo 1 && echo 2)"
#  exec_test "true || echo 1 && echo 2 || echo 3"
#  exec_test "(ls)"
#  exec_test "( ls )"
#
#  exec_test "true || (echo 1 && echo 2) || echo 3"
#  exec_test "true || (echo 1 && echo 2) && echo 3"
#  exec_test "(true || (echo 1 && echo 2) && echo 3)"
#  exec_test "true || ((echo 1 && echo 2) && echo 3)"
#
#  exec_test "( )"
#  exec_test " ls )"
#  exec_test "( ls "
#  exec_test "ls && (touch 1 && pwd) && "
#fi


if [[ "$1" != "" ]] && (( $TOTAL > 0)); then
  PROCENT=$(( $GOOD * 100  / $TOTAL ))
  if (( $PROCENT > 90)); then
    printf $GREEN
  elif (( $PROCENT <= 90 )) && (( $PROCENT > 60 )); then
    printf $YELLOW
  else
    printf $RED
  fi
  printf "\nPASS: $GOOD / $TOTAL (%%$PROCENT)$RESET\n"
fi

rm -f $pipe lol ls 1 test big_file
