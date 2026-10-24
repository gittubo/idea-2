# Northpine Packs: marketing handbook

> Placeholder brand. Replace every section below with your real business.

## Who we are
Northpine Packs makes durable, repairable backpacks for people who commute by
bike during the week and hike on weekends. Three-person marketing team, works in Slack.
Founded on one idea: buy one bag, keep it ten years.

## Who we sell to
- **Primary:** 25–40, city commuters, ride or walk to work, care about gear that lasts.
- **Secondary:** weekend hikers upgrading from a cheap first pack.
- They distrust hype and respond to specifics: weights, materials, warranty terms.

## Current launch
**The Ridge 28** (placeholder): 28 L roll-top, recycled ripstop, laptop sleeve,
lifetime repair program. Product page: `https://example.com/ridge-28`.
Only state specs listed in `brand/product-facts.md`. Never invent a number.

## How we sound
- Plain, warm, a little dry. Short sentences. Talk like a friend who works at a gear shop.
- Lead with the useful fact, then the feeling.
- One exclamation mark per email, at most.
- See `brand/voice.md` and the examples in `brand/best-emails/`.

## Banned words
game-changer, revolutionary, unleash, elevate, epic, must-have, insane,
"in today's fast-paced world", synergy, best-in-class

## Hard rules
1. **Never publish or send.** Emails stop as Kit drafts. Blog posts stop as
   WordPress `pending`. Social posts stop in the calendar.
2. **Airtable status:** you may set a row to `Needs approval`. Never set
   `Approved`, `Scheduled` or `Published`. Only humans (via n8n) do that.
3. **Every email** uses `templates/email.html`, which contains the unsubscribe
   link and mailing address. Never remove them (CAN-SPAM).
4. **AI people:** label every AI-generated person as AI. Our AI creator never
   poses as a customer or gives a testimonial (FTC rules). Size and fit claims
   are filmed with the real product, not generated.
5. **Links:** every email link gets UTM tags:
   `utm_source=kit&utm_medium=email&utm_campaign=<calendar-row-slug>`.
6. **Numbers you propose to change** (prices, discounts, send times) go in the
   report as proposals. Never change them yourself.
7. If something in a task conflicts with these rules, stop and say so in the
   calendar row's `Notes` field.

## Where things live
- Brand assets: `brand/`
- Email template: `templates/email.html`
- Content calendar schema: `calendar/schema.md`
- Playbooks: `.claude/skills/`
- WordPress helper: `scripts/wp-draft.sh` (reads `.env`)
