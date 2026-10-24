#!/usr/bin/env bash
# Upload an image to the WordPress media library and print its media id.
# Usage: scripts/wp-upload-image.sh path/to/image.jpg "Alt text"
set -euo pipefail
cd "$(dirname "$0")/.."
[ -f .env ] || { echo "Missing .env (copy .env.example)" >&2; exit 1; }
set -a; . ./.env; set +a
[ $# -ge 2 ] || { echo "Usage: $0 image.jpg \"Alt text\"" >&2; exit 2; }
file=$1 alt=$2
id=$(curl -fsS -u "$WP_USER:$WP_APP_PASSWORD" \
  -H "Content-Disposition: attachment; filename=$(basename "$file")" \
  --data-binary @"$file" "$WP_URL/wp-json/wp/v2/media" \
  | python3 -c 'import json,sys; print(json.load(sys.stdin)["id"])')
alt_json=$(python3 -c 'import json,sys; print(json.dumps({"alt_text":sys.argv[1]}))' "$alt")
curl -fsS -u "$WP_USER:$WP_APP_PASSWORD" -H 'Content-Type: application/json' \
  -d "$alt_json" "$WP_URL/wp-json/wp/v2/media/$id" > /dev/null
echo "$id"
