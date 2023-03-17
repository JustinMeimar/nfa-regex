re=$1
string=$2
if ../bin/regex $re $string | grep -q 'Accept String'; then
    exit 0 #0 means accept
else 
    exit 1 #1 means reject
fi