for i in {1..100000}
do
  ./tcpecho 127.0.0.1 hello$i 10011
done
