import json
import os

# Generate a single large JSON object with a large array
data = {"root": []}
target_size_mb = 1
target_size_bytes = target_size_mb * 1024 * 1024

# Add entries to the array
entry = {
    "id": 0,
    "name": "Item_0",
    "details": {"value": 0},
    "tags": [f"tag_{j}" for j in range(5)]
}

# Estimate size per entry by serializing one
entry_size = len(json.dumps(entry))
entries_needed = target_size_bytes // entry_size

# Fill the array
for i in range(entries_needed):
    data["root"].append({
        "id": i,
        "name": f"Item_{i}",
        "details": {"value": i * 10},
        "tags": [f"tag_{j}" for j in range(5)]
    })

# Write to file
with open("tests/large.json", "w") as f:
    json.dump(data, f)

# Change Size according to requirements
size_mb = os.path.getsize("tests/large.json") / 1024 / 1024
print(f"Generated large.json with size: {size_mb:.2f} MB")

if size_mb < target_size_mb:
    additional_entries = int((target_size_bytes - os.path.getsize("tests/large.json")) // entry_size)
    for i in range(additional_entries):
        data["root"].append({
            "id": i + entries_needed,
            "name": f"Item_{i + entries_needed}",
            "details": {"value": (i + entries_needed) * 10},
            "tags": [f"tag_{j}" for j in range(5)]
        })
    with open("tests/large.json", "w") as f:
        json.dump(data, f)
    size_mb = os.path.getsize("tests/large.json") / 1024 / 1024
    print(f"Adjusted large.json to size: {size_mb:.2f} MB")