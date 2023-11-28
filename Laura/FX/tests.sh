c++ client_test.cpp -o client0

a=0

while [ $a -lt 10 ]
do
	osascript -e 'tell application "iTerm" to tell current window to create tab with default profile' -e 'tell application "iTerm" to tell current session of current window to write text "cd Desktop/webserv_42/FX"' \
	-e 'tell application "iTerm" to tell current session of current window to write text "./client0"' \

	a=`expr $a + 1`
done

#osascript -e 'tell application "iTerm" to close current window'
