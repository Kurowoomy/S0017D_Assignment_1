cd ~/skrivbord/old-graph-proj/projects/MeshResources/code
touch copyright.txt
echo "//Made by an Emma :)" >> copyright.txt

# add all text from copyright.txt to all cc files in current folder
for f in *.cc; do
  cat copyright.txt "$f" > "$f.new"  &&  mv "$f.new" "$f"
done
# add all text from copyright.txt to all h files in current folder
for f in *.h; do
  cat copyright.txt "$f" > "$f.new"  &&  mv "$f.new" "$f"
done

# change extension from h to hpp
for f in *.h; do
    mv -- "$f" "${f%.h}.hpp"
done

# change float to double and back to all files in current folder
sed -i 's/float/double/g' *
sed -i 's/double/float/g' *