---
name: revise
description: Make version two of a piece after a reviewer requested changes in Slack. Triggered by n8n with an Airtable record id.
---
# Revise

1. Input: an Airtable `record_id`. Read the row, especially `Reviewer note`, `Channel`, `Draft link`, `Version`.
2. Read `CLAUDE.md`. If the note asks for something that breaks a hard rule (e.g. a fake
   testimonial, removing the unsubscribe link), don't do it; explain why in `Notes`.
3. Make the change in place:
   - Email: update the Kit draft.
   - Blog: update the WordPress post (keep `status=pending`).
   - Video ad: make a new take in Higgsfield through the connector; keep the AI label.
4. Increase `Version` by 1, clear nothing, add a one-line summary of what changed to `Notes`,
   set `Status = Needs approval` (n8n reposts it to Slack).
