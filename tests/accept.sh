re=$1
string=$2
if ../bin/regex $re $string | grep -q 'Accept String'; then
    # echo "accepted"
    exit 0;
else 
    # echo "rejected"    
    exit 1
fi