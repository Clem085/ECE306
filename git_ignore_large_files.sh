#!/bin/bash

# Set the size limit in bytes (100 MB)
SIZE_LIMIT=$((100 * 1024 * 1024))

# Find all files larger than the size limit
LARGE_FILES=$(find . -type f -size +100M)

# Check if there are large files
if [ -z "$LARGE_FILES" ]; then
    echo "No large files found."
    exit 0
fi

# Create or clear .gitignore
GITIGNORE=".gitignore"
touch $GITIGNORE

# Loop through the large files and add them to .gitignore
for FILE in $LARGE_FILES; do
    # Get the relative path and add it to .gitignore if not already present
    if ! grep -Fxq "$FILE" $GITIGNORE; then
        echo "$FILE" >> $GITIGNORE
        echo "Added $FILE to .gitignore"
    fi
done

echo "All large files have been added to .gitignore."
