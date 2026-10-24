---
name: launch-email
description: Write a product launch email in the brand voice and save it as a Kit draft. Use when a calendar row with Channel=Email needs a launch email.
---
# Launch email

1. Read `CLAUDE.md`, `brand/voice.md`, `brand/product-facts.md` and every file in `brand/best-emails/`.
2. Find the calendar row in Airtable. Set `Status = Drafting`.
3. Write: subject (under 45 chars) + 2 alternates, preview text, headline, 80–150 word body, one CTA.
   Only use specs from `brand/product-facts.md`. Check against the banned-words list.
4. Fill `templates/email.html`. Keep the footer (address + `{{ unsubscribe_url }}`) untouched.
   Add UTM tags to every link: `utm_source=kit&utm_medium=email&utm_campaign=<Slug>`.
5. In Kit, create a **draft** broadcast with that HTML. Do not schedule or send.
6. Update the row: `Draft link` = Kit draft URL, `Notes` = the alternate subject lines,
   `Status = Needs approval`.
