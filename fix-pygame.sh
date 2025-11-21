#!/bin/bash

echo "=== Fixing Pygame for Container ==="

# Clean and setup container
rm -rf my-container-root
mkdir -p my-container-root/app

# Copy chess files
cp "AI Chess Bot"/*.py my-container-root/app/

# Copy the entire venv
cp -r "AI Chess Bot/venv" my-container-root/

# Test the venv
echo "Testing venv..."
my-container-root/venv/bin/python3 -c "import pygame; print('✅ Pygame works in copied venv!')" || echo "❌ Pygame missing in venv"

# Create a runner script
cat > my-container-root/run-chess.sh << 'SCRIPT'
#!/bin/bash
echo "Starting chess from container..."
/venv/bin/python3 /app/ChessMain.py
SCRIPT

chmod +x my-container-root/run-chess.sh

echo "=== Setup Complete ==="
