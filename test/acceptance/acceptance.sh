#!/usr/bin/env bash

set -e
set -o pipefail


source test/common.sh


TEST_CHECK=0

main () {
    read_parameters $1
    echo "Check logic of application"
    sum=$( expr "10" + "20")
    ans="TEST INFO: 10 + 20 = $sum"
    logging_test "$ans"

    
    echo "Check the system hostname"
    name=$(hostname)

    logging_test "$name"

    if [[ "$TEST_CHECK" -gt "0" ]]; then
        echo " Test had $TEST_CHECK Failures"
        return 1
    else
        return 0
    fi

}
main "$@"