# Airtable content calendar

One base, one table: **Content**. One row per piece of content.

| Field | Type | Notes |
|---|---|---|
| Title | Single line text | e.g. "Ridge 28 launch email" |
| Slug | Single line text | Used as `utm_campaign`, e.g. `ridge-28-launch` |
| Channel | Single select | Email, Blog, Video ad, Social |
| Status | Single select | `Idea` → `Drafting` → `Needs approval` → `Changes requested` → `Approved` → `Scheduled` → `Published` |
| Publish date | Date | Proposed by Claude, confirmed by a human |
| Draft link | URL | Kit draft, WordPress preview, or Higgsfield asset |
| Version | Number | Starts at 1, `revise` adds 1 |
| Reviewer note | Long text | Filled by n8n from Slack "Request changes" |
| Notes | Long text | Claude's notes, including rule conflicts |
| AI label | Checkbox | Must be checked for any AI person or AI image |
| Owner | Collaborator | The human who approves |

**Claude may set:** `Drafting`, `Needs approval`.
**Only humans / n8n set:** `Approved`, `Scheduled`, `Published`, `Changes requested`.

A starter import file is in `calendar/starter-rows.csv`.
