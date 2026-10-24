# AI marketing employee (starter kit)

A Claude Code workspace that drafts marketing work inside your real tools while
your team approves every piece in Slack. The brand here, **Northpine Packs**, is
a fictional placeholder. Replace everything in `CLAUDE.md` and `brand/` with your own.

## How it works

1. **Handbook.** `CLAUDE.md` tells Claude who you are, who you sell to, how you
   sound, and the hard rules. `brand/` holds the logo, colors, fonts, photos and
   your best past emails.
2. **Tools.** Each tool is a connector (MCP server) you add once in your Claude
   account (Customize → Connectors): Kit, Airtable, Slack, Higgsfield, Google
   Analytics. WordPress uses an application password in `.env`
   (see `scripts/wp-draft.sh`).
3. **Calendar.** One Airtable row per piece of content, with a status column
   (`calendar/schema.md`). Claude can move a row to `Needs approval` and never past it.
4. **Jobs.** Skills in `.claude/skills/` are saved playbooks:
   | Skill | What it does | Where the output stops |
   |---|---|---|
   | `launch-email` | Writes a launch email in the brand voice | Kit **draft** |
   | `win-back` | Writes a sequence for subscribers with no opens in 90 days | Kit **draft** sequence |
   | `blog-post` | Writes a post with image, internal links, signup form | WordPress **Pending** |
   | `revise` | Reads a reviewer's note and makes version two | Back to `Needs approval` |
   | `weekly-report` | Monday numbers + proposed fixes | Slack, fixes need approval |
5. **Cloud + sign-off.** Push this folder to GitHub and turn the skills into
   scheduled Claude Code tasks in the cloud. n8n posts every `Needs approval`
   row to Slack with **Approve / Request changes** buttons; Approve schedules it,
   Request changes saves the note and wakes the `revise` task.

## Setup checklist

- [ ] Rewrite `CLAUDE.md` and `brand/` for your real brand
- [ ] Put your real mailing address and unsubscribe tag in `templates/email.html`
- [ ] Create the Airtable base from `calendar/schema.md`
- [ ] Add the Kit, Airtable, Slack, Higgsfield and Google Analytics connectors in your Claude account
- [ ] `cp .env.example .env` and fill in the WordPress application password
- [ ] Set up the n8n Slack approval flow (`calendar/approval-flow.md`)
