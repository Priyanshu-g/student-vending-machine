count = 0
while true; do
    ./soda ./configs/soda.config
    ((count++))
    echo "DATA TEST COUNT HERE: $count"
done