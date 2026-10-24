#!/usr/bin/env bash
# Create a WordPress post with status "pending" (never "publish").
# Usage: scripts/wp-draft.sh "Title" path/to/body.html [featured_media_id]
set -euo pipefail
cd "$(dirname "$0")/.."
[ -f .env ] || { echo "Missing .env (copy .env.example)" >&2; exit 1; }
set -a; . ./.env; set +a
[ $# -ge 2 ] || { echo "Usage: $0 \"Title\" body.html [featured_media_id]" >&2; exit 2; }
title=$1 body_file=$2 media=${3:-0}
payload=$(python3 -c 'import json,sys; print(json.dumps({"title":sys.argv[1],"content":open(sys.argv[2]).read(),"status":"pending","featured_media":int(sys.argv[3])}))' "$title" "$body_file" "$media")
curl -fsS -u "$WP_USER:$WP_APP_PASSWORD" -H 'Content-Type: application/json' \
  -d "$payload" "$WP_URL/wp-json/wp/v2/posts" \
  | python3 -c 'import json,sys; p=json.load(sys.stdin); print(p["id"], p["status"], p["link"])'
