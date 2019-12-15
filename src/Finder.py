# from progressbar import ProgressBar
from tqdm import tqdm, trange
import os
import sys
import hashlib
import pickle
import json


def chunk_reader(fobj, chunk_size=1024):
    """
    Generator that reads a file in chunks of bytes
    """
    while True:
        chunk = fobj.read(chunk_size)
        if not chunk:
            return
        yield chunk


def get_hash(filename, first_chunk_only=False, hash=hashlib.sha1):
    hashobj = hash()
    file_object = open(filename, 'rb')

    if first_chunk_only:
        hashobj.update(file_object.read(1024))
    else:
        for chunk in chunk_reader(file_object):
            hashobj.update(chunk)

    hashed = hashobj.digest()
    file_object.close()

    return hashed


def check_for_duplicates(paths, hash=hashlib.sha1):
    hashes_by_size = {}
    hashes_on_1k = {}
    hashes_full = {}

    for path in paths:
        for dirpath, dirnames, filenames in tqdm(os.walk(path), desc="Hashes by Size", ascii=True):
            # for filename in tqdm(filenames, desc="Hashes by Size", leave=False, ascii=True):
            for filename in filenames:
                full_path = os.path.join(dirpath, filename)
                try:
                    # if the target is a symlink (soft one), this will
                    # dereference it - change the value to the actual target file
                    full_path = os.path.realpath(full_path)
                    file_size = os.path.getsize(full_path)
                except (OSError,):
                    # not accessible (permissions, etc) - pass on
                    continue

                duplicate = hashes_by_size.get(file_size)

                if duplicate:
                    hashes_by_size[file_size].append(full_path)
                else:
                    # create the list for this file size
                    hashes_by_size[file_size] = []
                    hashes_by_size[file_size].append(full_path)

    # For all files with the same file size, get their hash on the 1st 1024 bytes
    for __, files in tqdm(hashes_by_size.items(), desc="Hashes By Size -> 1k", ascii=True):
        if len(files) < 2:
            continue    # this file size is unique, no need to spend cpy cycles on it

        for filename in tqdm(files, desc="File", leave=False, ascii=True):
            try:
                small_hash = get_hash(filename, first_chunk_only=True)
            except (OSError,):
                # the file access might've changed till the exec point got here
                continue

            duplicate = hashes_on_1k.get(small_hash)
            if duplicate:
                hashes_on_1k[small_hash].append(filename)
            else:
                # create the list for this 1k hash
                hashes_on_1k[small_hash] = []
                hashes_on_1k[small_hash].append(filename)

    # For all files with the hash on the 1st 1024 bytes, get their hash on the full file - collisions will be duplicates
    for __, files in tqdm(hashes_on_1k.items(), desc="Hashes by 1k -> Full", ascii=True):
        if len(files) < 2:
            continue    # this hash of fist 1k file bytes is unique, no need to spend cpy cycles on it

        for filename in tqdm(files, desc="File", leave=False, ascii=True):
            try:
                full_hash = get_hash(filename, first_chunk_only=False)
            except (OSError,):
                # the file access might've changed till the exec point got here
                continue

            duplicate = hashes_full.get(full_hash)
            if not duplicate:
                # print(f"Duplicate found: {filename} and {duplicate}")
                hashes_full[full_hash] = filename

    # Dump to Json

    # hashes_by_size
    # hashes_on_1k
    # hashes_full
    hashes_on_1k = {str(k): v for (k, v) in hashes_on_1k.items()}
    hashes_full = {str(k): v for (k, v) in hashes_full.items()}

    h_s = {}
    h_1k = {}
    h_full = {}

    # Remove non important data
    for (k, v) in hashes_by_size.items():
        if len(v) > 1:
            h_s[k] = v

    for (k, v) in hashes_on_1k.items():
        if len(v) > 1:
            h_1k[k] = v

    for (k, v) in hashes_full.items():
        if len(v) > 1:
            h_full[k] = v

    with open("hashes_by_size.json", "w") as f:
        json.dump(h_s, f, indent=4)

    with open("hashes_on_1k.json", "w") as f:
        json.dump(h_1k, f, indent=4)

    with open("hashes_full.json", "w") as f:
        json.dump(h_full, f, indent=4)

    with open("size.p", "wb") as f:
        pickle.dump(h_s, f, protocol=pickle.HIGHEST_PROTOCOL)

    with open("1k.p", "wb") as f:
        pickle.dump(h_1k, f, protocol=pickle.HIGHEST_PROTOCOL)

    with open("full.p", "wb") as f:
        pickle.dump(h_full, f, protocol=pickle.HIGHEST_PROTOCOL)


if sys.argv[1:]:
    check_for_duplicates(sys.argv[1:])
else:
    print("Please pass the paths to check as parameters to the script")
