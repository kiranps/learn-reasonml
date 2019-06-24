[@react.component]
let make = (~name) =>
  switch (name) {
  | "play" =>
    <svg
      className="fill-current text-gray-600 w-6 h-6 mr-1" viewBox="0 0 20 20">
      <polygon points="4 4 16 10 4 16" />
    </svg>
  | "format" =>
    <svg
      className="fill-current text-gray-600 w-4 h-4 mr-1" viewBox="0 0 20 20">
      <path
        fill="#000000"
        d="M3,3H21V5H3V3M3,7H15V9H3V7M3,11H21V13H3V11M3,15H15V17H3V15M3,19H21V21H3V19Z"
      />
    </svg>
  };