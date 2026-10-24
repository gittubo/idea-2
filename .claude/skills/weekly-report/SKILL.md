---
name: weekly-report
description: Write the Monday marketing report from Google Analytics, Search Console, Kit and Blotato numbers, with proposed fixes that need approval.
---
# Weekly report

1. Input: the numbers n8n sent, or pull them yourself via the Google Analytics connector and Kit.
2. Report, last 7 days vs the 7 before:
   - Sessions and signups by `utm_campaign` (from GA)
   - Kit: sends, open rate, click rate, unsubscribes, new subscribers
   - Blog: top 5 posts by sessions; Search Console queries that moved
   - Social/ads (Blotato): top 3 posts
3. End with **at most 3 proposed fixes**, each as a new Airtable row with
   `Status = Needs approval` and the reasoning in `Notes`. Do not apply any fix.
4. Post the report to `#marketing` in Slack. Keep it under 250 words and lead with the one thing that matters most.
