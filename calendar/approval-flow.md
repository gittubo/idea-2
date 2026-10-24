# n8n + Slack approval flow

**n8n needs its own keys** (not Claude's): Airtable token, Kit API key,
WordPress application password, Blotato key, and a Slack app with
**Interactivity** turned on (Request URL = your n8n webhook).

## Workflow 1: post for approval
1. Trigger: Airtable, row where `Status = Needs approval` (poll every 5 min).
2. Slack: post to `#marketing-approvals` with Title, Channel, Version,
   Draft link, AI label, and two buttons: **Approve** / **Request changes**.

## Workflow 2: handle the button
1. Trigger: Slack interactivity webhook.
2. **Approve** → set `Status = Approved`, then by Channel:
   - Email: schedule the Kit broadcast at `Publish date`.
   - Blog: set the WordPress post to `future` at `Publish date`.
   - Video/Social: schedule in Blotato with the AI label on.
   Then set `Status = Scheduled`.
3. **Request changes** → open a Slack modal for the note, save it to
   `Reviewer note`, set `Status = Changes requested`, then POST to the
   `revise` cloud task's web address with `{ "record_id": "<row id>" }`.

## Workflow 3: Monday report
Every Monday 08:00: pull Google Analytics, Search Console, Kit and Blotato
stats, then wake the `weekly-report` cloud task with the numbers.
