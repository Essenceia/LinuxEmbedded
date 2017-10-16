#!/bin/bash
echo "Uses logins whom UID are multiples of two the followings:"
grep '^[^:]*:[^:]*:[0-9]*[02468]:.' /etc/passwd | cut -d: -f1
