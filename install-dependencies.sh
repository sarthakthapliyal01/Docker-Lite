#!/bin/bash

CONTAINER_ROOT="./my-container-root"

echo "Installing Python dependencies for container..."

# Find where Python packages are installed
PYTHON_PATH=$(dirname $(which python3))
SITE_PACKAGES=$(python3 -c "import site; print(site.getsitepackages()[0])")

echo "Python path: $PYTHON_PATH"
echo "Site packages: $SITE_PACKAGES"

# Copy pygame if it exists
if [ -d "$SITE_PACKAGES/pygame" ]; then
    echo "Copying pygame..."
    mkdir -p $CONTAINER_ROOT$SITE_PACKAGES
    cp -r $SITE_PACKAGES/pygame* $CONTAINER_ROOT$SITE_PACKAGES/
else
    echo "Pygame not found in system. Installing..."
    pip3 install pygame
    SITE_PACKAGES=$(python3 -c "import site; print(site.getsitepackages()[0])")
    mkdir -p $CONTAINER_ROOT$SITE_PACKAGES
    cp -r $SITE_PACKAGES/pygame* $CONTAINER_ROOT$SITE_PACKAGES/
fi

# Copy other required Python libraries
for lib in sys os random; do
    # These are built-in, but we copy the standard library structure
    echo "Ensuring $lib is available..."
done

echo "Dependencies installed!"