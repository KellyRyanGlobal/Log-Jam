#!/usr/bin/env bash

set -e

main() {

    if [[ -z $1  ]]; then        
        echo " Host was not passed as a param (see config)"
        return 1        
    else
        
        mkdir -p input output
        sleep 5  
        
        make -C src 
        if [[ $? > 0 ]]; then 
            echo "Build Failed" && return 1
        else
            echo "Build successful" 
        fi
          
        # adding in 5 seconds in case system is slow    
         
    fi
    return 0
}


main "$@"