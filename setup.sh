#!/bin/bash
echo "=== Setting up Container Files ==="

# Clean and create fresh
rm -rf my-container-root
mkdir -p my-container-root/app

# Copy chess files
echo "Copying chess files..."
cp "AI Chess Bot"/*.py my-container-root/app/
cp -r "AI Chess Bot/pieceimages" my-container-root/app/

# Verify
echo "=== Verification ==="
echo "Python files:"
ls my-container-root/app/*.py
echo ""
echo "Piece images:"
ls my-container-root/app/pieceimages/*.png | head -5

echo "=== Setup Complete ==="