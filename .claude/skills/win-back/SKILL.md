---
name: win-back
description: Build a win-back email sequence in Kit for subscribers with no opens in 90 days, saved as drafts. Use for the win-back calendar row.
---
# Win-back sequence

1. Read `CLAUDE.md` and `brand/voice.md`.
2. In Kit, check for a tag `cold-90d`. If it doesn't exist, create it and write in `Notes`
   that a human must add the Kit rule "no opens in 90 days → tag cold-90d → start sequence".
   Do not create automations that send.
3. Write 3 emails, each with `templates/email.html` and UTM tags (`utm_campaign=<Slug>`):
   - Day 0: "Still want to hear from us?" plus one useful tip, not a sale.
   - Day 4: the most-read item from the last 90 days.
   - Day 9: a link that tags `stay-subscribed` if clicked; say plainly that otherwise
     we'll stop emailing.
4. Create the sequence in Kit as **inactive/draft**.
5. Update the row: `Draft link`, `Status = Needs approval`.
