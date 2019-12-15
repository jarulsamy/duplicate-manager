import os
import sys
from pathlib import Path
from PIL import Image, ExifTags
import datetime
import time
import uuid
import shutil
from duplicate_removal import check_for_duplicates
from tqdm import tqdm

# Destination should be EMPTY to avoid duplicates.
INPUT_PATH = Path(r"D:\hootoo")
# DESTINATION_DIR = Path(r"C:\Users\Joshua\Desktop\hootoo_organized")
DESTINATION_DIR = Path(r"D:\hootoo_organized")

# for dirpath, dirnames, filenames in tqdm(os.walk(INPUT_PATH), desc="Dir", ascii=True):
for dirpath, dirnames, filenames in os.walk(INPUT_PATH):
    # for filename in tqdm(filenames, desc="File", leave=False, ascii=True):
    for filename in filenames:

        filename = Path(filename)
        dirpath = Path(dirpath)
        full_path = Path.joinpath(dirpath, filename)

        try:
            # Grab modifcation date - basically creation, metadata is not always accurate
            cdate = time.ctime(os.path.getmtime(full_path))

            # Convert to datetime object
            date = datetime.datetime.strptime(cdate, "%a %b %d %H:%M:%S %Y")

            # Find and create the dir of image
            year_path = Path.joinpath(DESTINATION_DIR, str(date.year))
            month_path = Path.joinpath(year_path, str(date.month))
            month_path.mkdir(parents=True, exist_ok=True)

            # Create new unique filename
            new_filename = str(uuid.uuid4().hex) + filename.suffix
            new_path = Path.joinpath(month_path, new_filename)

            shutil.copy2(full_path, new_path)
            print(f"{full_path} -> {new_path}")

        # Unless it's not an image
        except OSError as e:
            pass

        # Metadata extraction fail
        except (AttributeError, KeyError) as e:
            pass

check_for_duplicates([DESTINATION_DIR])
